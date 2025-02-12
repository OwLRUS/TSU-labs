const express = require('express');
const bodyParser = require('body-parser');
const crypto = require('crypto');
const fs = require('fs');
const path = require('path');

const app = express();
const port = 3000;

// Путь к файлу данных
const usersFile = path.join(__dirname, 'users.json');
const signsFile = path.join(__dirname, 'signs.json');
const prKeyFile = path.join(__dirname, 'PriK.pem');
const pbKeyFile = path.join(__dirname, 'PubK.pem');

// Инициализации файлов
if (!fs.existsSync(usersFile)) {
    fs.writeFileSync(usersFile, JSON.stringify({}), 'utf8');
}

if (!fs.existsSync(signsFile)) {
    fs.writeFileSync(signsFile, JSON.stringify({}), 'utf8');
}

if(!fs.existsSync(prKeyFile) || !fs.existsSync(pbKeyFile)) {
    if (!fs.existsSync(prKeyFile) && !fs.existsSync(pbKeyFile)) {
        console.log('Generating Private Key and Public Key...');

        const { publicKey, privateKey } = crypto.generateKeyPairSync('rsa', {
            modulusLength: 2048, // Длина ключа (рекомендуется не менее 2048 бит)
            publicKeyEncoding: {
              type: 'spki', // Кодировка открытого ключа (Subject Public Key Info)
              format: 'pem' // Формат PEM
            },
            privateKeyEncoding: {
              type: 'pkcs8', // Кодировка закрытого ключа (PKCS #8)
              format: 'pem', // Формат PEM
            }
        });

        fs.writeFileSync(prKeyFile, privateKey);
        fs.writeFileSync(pbKeyFile, publicKey);

        console.log('Private Key and Public Key were successfully generated and written to files.');
    } else {
        console.log('system error');
    }
}

const publicKey = fs.readFileSync('PubK.pem', 'utf8');
const privateKey = fs.readFileSync('PriK.pem', 'utf8');

if(!publicKey || !privateKey){
    console.log('keys are not exist!');
}

// Функция для загрузки пользователей
function loadUsers() {
    const data = fs.readFileSync(usersFile, 'utf8');
    return JSON.parse(data);
}

// Функция для загрузки подписей
function loadSigns() {
    const data = fs.readFileSync(signsFile, 'utf8');
    return JSON.parse(data);
}

// Функция для сохранения пользователей
function saveUsers(users) {
    fs.writeFileSync(usersFile, JSON.stringify(users, null, 2), 'utf8');
}

// Функция для сохранения подписей
function saveSigns(signs) {
    fs.writeFileSync(signsFile, JSON.stringify(signs, null, 2), 'utf8');
}

// Миддлвар для парсинга JSON
app.use(bodyParser.json());

// Отдаём статические файлы из папки public
app.use(express.static(path.join(__dirname, 'public')));

// Получение зерна для хэширования на клиенте
app.post('/seed', (req, res) => {
    const seed = crypto.randomBytes(8).toString('hex');
    
    res.status(200).json({ seed });
});

// Регистрация нового пользователя
app.post('/make', (req, res) => {
    const { username, email, file, seed } = req.body;

    if (!username || !email || !file || !seed) {
        return res.status(400).send(`Некорректные данные в запросе: ${username}, ${email}, ${file}, ${seed}`);
    }

    const users = loadUsers();
    const signs = loadSigns();

    if (!users[email]) {
        const id = crypto.createHmac('sha256', seed).update(email).digest('hex');
        users[email] = { name : username, id, seed };

        saveUsers(users);
    }

    const Sign = crypto.createSign('SHA256');
    
    if(!signs['last']) {
        const n = 1;

        const time = Date.now();

        const initIdSalt = crypto.randomBytes(8).toString('hex');
        const initId = crypto.createHmac('sha256', seed).update(initIdSalt).digest('hex');

        const initHashSalt = crypto.randomBytes(8).toString('hex');
        const initHash = crypto.createHmac('sha256', seed).update(initHashSalt).digest('hex');

        const initSignSalt = crypto.randomBytes(8).toString('hex');
        const initSign = crypto.createHmac('sha256', seed).update(initSignSalt).digest('hex');

        const initLfuncSalt = crypto.randomBytes(8).toString('hex');
        const initLfunc = crypto.createHmac('sha256', seed).update(initLfuncSalt).digest('hex');

        const Lfunc = crypto.createHmac('sha256', seed).update(initId + initHash + initSign + initLfunc).digest('hex');
        
        Sign.update(`${n}, ${username}, ${file}, ${time}, ${Lfunc}`);
        Sign.end();
        const result = Sign.sign(privateKey, 'hex');
        
        signs['last'] = { n, name : username, hash : file, time, id : users[email].id, sign : result, L : Lfunc};
        signs[n] = { name : username, hash : file, time, id : users[email].id, sign : result, L : Lfunc};
        signs[0] = { name : "init", hash : initHash, time : time - 1, id : initId, sign : initSign, L : initLfunc};

        saveSigns(signs);

        res.status(200).json({n, time, result});
    } else {
        const n = signs['last'].n + 1;
        const lastId = signs['last'].id;
        const lastHash = signs['last'].hash;
        const lastSign = signs['last'].sign;
        const lastLfunc = signs['last'].L;

        const time = Date.now();

        const Lfunc = crypto.createHmac('sha256', seed).update(lastId + lastHash + lastSign + lastLfunc).digest('hex');

        Sign.update(`${n}, ${username}, ${file}, ${time}, ${Lfunc}`);
        Sign.end();
        const result = Sign.sign(privateKey, 'hex');

        signs['last'] = { n, name : username, hash : file, time, id : users[email].id, sign : result, L : Lfunc};
        signs[n] = { name : username, hash : file, time, id : users[email].id, sign : result, L : Lfunc};

        saveSigns(signs);

        res.status(200).json({n, time, result});
    }
});

// Авторизация пользователя
app.post('/check', (req, res) => {
    const { file, iter } = req.body;
    const signs = loadSigns();
    const users = loadUsers();
    
    const { n, result : sign2verify, time } = file;

    if(n && sign2verify && time) {
        if(signs[n]) {
            if(signs[n].time != time) {
                return res.status(200).send('Given time does not match!');
            }

            let lwBorder = 0;
            let grBorder = 0;
            const maxN = signs['last'].n;

            if(maxN - iter < 1) {
                lwBorder = Math.round(maxN / 2 - 1);
                console.log('Set lower border: ', lwBorder);
            } else {
                lwBorder = iter;
            }

            if(n + iter > maxN) {
                grBorder = Math.round(maxN / 2 - 1);
                console.log('Set greater border: ', grBorder);
            } else {
                grBorder = iter;
            }


            let verSign = sign2verify;

            for(let i = 0; i < lwBorder; i++) {
                const Verify = crypto.createVerify('SHA256');
                const data = signs[n - i];
                const prevData = signs[n - 1 - i];

                const targetId = signs[n - i].id;
                let seed = null;
                for (const email in users) {
                if (users[email].id === targetId) {
                    seed = users[email].seed;
                    break;
                }
                }

                if(prevData)
                {
                    Verify.update(`${n - i}, ${data.name}, ${data.hash}, ${data.time}, ${data.L}`);
                    Verify.end();
                    
                    if(!Verify.verify(publicKey, verSign, 'hex')) {
                        return res.status(200).send('Sign is falsified!');
                    }

                    const exLfunc = crypto.createHmac('sha256', seed).update(prevData.id + prevData.hash + prevData.sign + prevData.L).digest('hex');
                    if(exLfunc != data.L)
                    {
                        console.log('1: Stored L function: ', data.L);
                        console.log('1: Expected L function: ', exLfunc);
                        return res.status(200).send('Chain is falsified!');
                    }

                    if(data.time < prevData.time)
                    {
                        return res.status(200).send('Time is falsified!');
                    }

                    verSign = prevData.sign;
                } else break;
            }

            for(let i = 0; i < grBorder; i++) {
                const Verify = crypto.createVerify('SHA256');
                const data = signs[n + i];
                const nextData = signs[n + 1 + i];

                const targetId = signs[n + 1 + i].id;
                let seed = null;
                for (const email in users) {
                if (users[email].id === targetId) {
                    seed = users[email].seed;
                    break;
                }
                }

                if(nextData)
                {
                    Verify.update(`${n + 1 + i}, ${nextData.name}, ${nextData.hash}, ${nextData.time}, ${nextData.L}`);
                    Verify.end();

                    verSign = nextData.sign;
                    
                    if(!Verify.verify(publicKey, verSign, 'hex')) {
                        return res.status(200).send('Sign is falsified!');
                    }

                    const exLfunc = crypto.createHmac('sha256', seed).update(data.id + data.hash + data.sign + data.L).digest('hex');
                    if(exLfunc != nextData.L)
                    {
                        console.log('2: Stored L function: ', nextData.L);
                        console.log('2: Expected L function: ', exLfunc);
                        return res.status(200).send('Chain is falsified!');
                    }

                    if(data.time > nextData.time)
                    {
                        return res.status(200).send('Time is falsified!');
                    }
                } else break;
            }

            return res.status(200).send('The time stamp is actual');
        } else {
            return res.status(400).send('Can not find the sign!');
        }
    } else {
        res.status(400).send('Can not read the file!');
    }
});

// Запуск сервера
app.listen(port, () => {
    console.log(`Сервер запущен на http://localhost:${port}`);
});