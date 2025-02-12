using System;
using System.Collections;
using System.Collections.Generic;

namespace testC
{
    class Program
    {
        static void Main(string[] args)
        {
            Random random = new Random();
            /*
            //1) Создайте пустой список S1. Добавьте элемент 1 в голову, добавьте элемент 10 в хвост. Выведите S1 на экран(используя функцию Print)
            Console.WriteLine("1) Создайте пустой список S1. Добавьте элемент 1 в голову, добавьте элемент 10 в хвост. Выведите S1 на экран(используя функцию Print)");
            MyList<int> S1 = new MyList<int>();
            S1.Push_front(1);
            S1.Push_back(10);
            Console.Write("S1: ");
            foreach (int i in S1)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            //2) Создайте список S2 из 6 случайных чисел. Выведите список S2 на экран, используя потоковый вывод.Найдите в S2 max и min элемент.Отсортируйте S2.
            Console.WriteLine("2) Создайте список S2 из 6 случайных чисел. Выведите список S2 на экран, используя потоковый вывод.Найдите в S2 max и min элемент.Отсортируйте S2.");
            MyList<int> S2 = new MyList<int>();
            for (int i = 0; i < 6; i++)
                S2.Push_back(random.Next()%100);

            Console.Write("S2: ");
            foreach (int i in S2)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            Console.WriteLine("Min: " + S2.Min().ToString());
            Console.WriteLine("Max: " + S2.Max().ToString());
            S2.Sort();
            Console.Write("отсортированный S2: ");
            foreach (int i in S2)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            //3) Найдите в S2 2 - й элемент и выведите его на экран. Удалите 2 - й элемент списка S2.
            Console.WriteLine("3) Найдите в S2 2 - й элемент и выведите его на экран. Удалите 2 - й элемент списка S2.");
            Console.WriteLine("второй в S2: " + S2[1].ToString());
            S2.Pop(1);

            //4) Найдите в S2 6 - й элемент(т.е.убедитесь, что в списке осталось меньше 6 элементов).Удалите элемент из хвоста S2.
            Console.WriteLine("4) Найдите в S2 6 - й элемент(т.е.убедитесь, что в списке осталось меньше 6 элементов).Удалите элемент из хвоста S2.");
            if (S2.FindByIndex(5) == null)
                Console.WriteLine("Шестого элемента не существует");
            else
                Console.WriteLine("Шестой элемент: "+S2[5].ToString());
            S2.Pop_back();
            Console.Write("S2: ");
            foreach (int i in S2)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            //5) Создайте пустой список S3. Инициализируйте его значением S1. Проверьте равенство списков S1  и S3. Проверьте, есть ли в S3 элемент 15.
            Console.WriteLine("5) Создайте пустой список S3. Инициализируйте его значением S1. Проверьте равенство списков S1  и S3. Проверьте, есть ли в S3 элемент 15.");
            MyList<int> S3 = S1.Copy();
            if (S1 == S3)
                Console.WriteLine("Списки идентичны");
            else
                Console.WriteLine("Списки не идентичны");
            if (S3.Find(15) == null)
                Console.WriteLine("В списке не содержится 15");
            else
                Console.WriteLine("В списке содержится 15");

            //6) Удалите элемент из головы списка S3. Удалите из списка S3 элемент 10.Выведите S3 на экран.Проверьте S3 на пустоту.
            Console.WriteLine("6) Удалите элемент из головы списка S3. Удалите из списка S3 элемент 10.Выведите S3 на экран.Проверьте S3 на пустоту.");
            S3.Pop_front();
            S3.Remove(10);
            if (S3.Empty())
                Console.WriteLine("Список пуст");
            else
                Console.WriteLine("Список не пуст");

            //7) Создайте последовательность из 6 случайных чисел, меньших 20.Превратите ее в список S4 и выведите его на экран.Проверьте, есть ли в S4 элемент 25.Добавьте элемент 25 четвертым в список.
            Console.WriteLine("7) Создайте последовательность из 6 случайных чисел, меньших 20.Превратите ее в список S4 и выведите его на экран.Проверьте, есть ли в S4 элемент 25.Добавьте элемент 25 четвертым в список.");
            int[] arr = new int[6];
            for (int i = 0; i < 6; i++)
                arr[i] = random.Next()%20;
            MyList<int> S4 = new MyList<int>(arr);
            Console.Write("S4: ");
            foreach (int i in S4)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            if (S4.Find(25) == null)
                Console.WriteLine("В списке не содержится 25");
            else
                Console.WriteLine("В списке содержится 25");
            S4.Insert(3, 25);

            //8) Создайте список S5, проинициализировав его при создании списком S2. Выведите S5 на экран. Проверьте, есть ли в S5 элемент 4 и если есть – удалите его, если нет – добавьте элемент 4 в хвост.
            Console.WriteLine("8) Создайте список S5, проинициализировав его при создании списком S2. Выведите S5 на экран. Проверьте, есть ли в S5 элемент 4 и если есть – удалите его, если нет – добавьте элемент 4 в хвост.");
            MyList<int> S5 = S2.Copy();
            Console.Write("S5: ");
            foreach (int i in S5)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            if (S5.Find(4) == null)
            {
                Console.WriteLine("В списке не содержится 4");
                S5.Push_back(4);
            }
            else
            {
                Console.WriteLine("В списке содержится 4");
                S5.Remove(4);
            }

            //9) Измените S5, записав в него 4 числа: 11, 12, 13, 14(числа вводятся с клавиатуры).Сравните S5 и S4.
            Console.WriteLine("9) Измените S5, записав в него 4 числа: 11, 12, 13, 14(числа вводятся с клавиатуры).Сравните S5 и S4.");
            Console.WriteLine("Введите 4 числа");
            for(int i=0; i<4; i++)
            {
                int num;
                if(!int.TryParse(Console.ReadLine(),out num))
                {
                    Console.WriteLine("Ошибка ввода");
                    Environment.Exit(0);
                }

                S5.Push_back(num);
            }
            if (S4 == S5)
                Console.WriteLine("Списки идентичны");
            else
                Console.WriteLine("Списки не идентичны");

            //10) Добавьте в хвост S5 список S4. Добавьте в голову S5 список S1.
            Console.WriteLine("10) Добавьте в хвост S5 список S4. Добавьте в голову S5 список S1.");
            S5 = S5 + S4;
            S5 = S1 + S5;
            Console.Write("S5: ");
            foreach (int i in S5)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            */
            
            //////////////////////////////////
            //1) Создайте множество S1, из 10 случайных чисел. Выведите S1 на экран(используя функцию Print).
            Console.WriteLine("1) Создайте множество S1, из 10 случайных чисел. Выведите S1 на экран(используя функцию Print).");
            MySet<int> s1 = new MySet<int>();
            for (int i = 0; i < 10; i++)
                s1.Add(random.Next() % 100);
            Console.Write("s1: ");
            foreach (int i in s1)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            //2) Создайте множество S2 и инициализируйте его(при создании) значением S1.  Выведите S2 на экран(используйте потоковый вывод). Проверьте равенство множеств S1  и S2.
            Console.WriteLine("2) Создайте множество S2 и инициализируйте его(при создании) значением S1.  Выведите S2 на экран(используйте потоковый вывод). Проверьте равенство множеств S1  и S2.");
            MySet<int> s2 = s1.Copy();
            Console.Write("s2: ");
            foreach (int i in s2)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            if (s1 == s2)
                Console.WriteLine("Множества равны");
            else
                Console.WriteLine("Множества не равны");

            //3) Проверьте, есть ли в S1 элемент 5.Создайте множество S3, которое получается удалением/ добавлением из S1 элемента 5.Проверьте, что S1 и S3 – не равны.
            Console.WriteLine("3) Проверьте, есть ли в S1 элемент 5.Создайте множество S3, которое получается удалением/ добавлением из S1 элемента 5.Проверьте, что S1 и S3 – не равны.");
            MySet<int> s3 = s1.Copy();
            if (s1.Contains(5))
            {
                Console.WriteLine("Пятерка содержится");
                s3 = s3 / 5;
            }
            else
            {
                Console.WriteLine("Пятерка не содержится");
                s3 = s3 + 5;
            }
            if (s1 == s3)
                Console.WriteLine("Множества равны");
            else
                Console.WriteLine("Множества не равны");

            //4) Создайте пустое множество S4. Проверьте его на пустоту.  Добавьте в S4 последовательно числа 5, 10, 15, 5.Выведите S4 на экран.
            Console.WriteLine("4) Создайте пустое множество S4. Проверьте его на пустоту.  Добавьте в S4 последовательно числа 5, 10, 15, 5.Выведите S4 на экран.");
            MySet<int> s4 = new MySet<int>();
            if (s4.Empty())
                Console.WriteLine("Множество пустое");
            else
                Console.WriteLine("Множество не пустое");
            s4.Add(5);
            s4.Add(10);
            s4.Add(15);
            s4.Add(5);
            Console.Write("s4: ");
            foreach (int i in s4)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            //5) Создайте пустое множество S5.  Инициализируйте его множеством S4.  Проверьте, что во множестве S5 есть элемент 15 и удалите его.Выведите получившееся множество на экран.
            Console.WriteLine("5) Создайте пустое множество S5.  Инициализируйте его множеством S4.  Проверьте, что во множестве S5 есть элемент 15 и удалите его.Выведите получившееся множество на экран.");
            MySet<int> s5 = new MySet<int>();
            s5 = s4.Copy();
            if (s5.Contains(15))
                Console.WriteLine("15 содержится");
            else
                Console.WriteLine("15 не содержится");
            s5.Remove(15);
            Console.Write("s5: ");
            foreach (int i in s5)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            //6) Создайте список T, из 20 случайных чисел. Выведите T на экран. Создайте из T множество S6.Выведите S6 на экран. Определите количество элементов в S6.
            Console.WriteLine("6) Создайте список T, из 20 случайных чисел. Выведите T на экран. Создайте из T множество S6.Выведите S6 на экран. Определите количество элементов в S6.");
            MyList<int> T = new MyList<int>();
            for (int i = 0; i < 20; i++)
                T.Push_back(random.Next() % 100);

            Console.Write("T: ");
            foreach (int i in T)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            MySet<int> s6 = new MySet<int>(T);
            Console.Write("s6: ");
            foreach (int i in s6)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            Console.WriteLine(s6.Size());

            //8) Создайте множество S9 ={ 1,3,5,7,9,11,13,15,17,19,21,23,25,27,29} (используйте потоковый ввод).  Найдите V1 = S6 ∩ S9,  V2 = S6 ∪ S9,  V3 = S6 \ S9.
            Console.WriteLine("8) Создайте множество S9 ={ 1,3,5,7,9,11,13,15,17,19,21,23,25,27,29} (используйте потоковый ввод).  Найдите V1 = S6 пересечение S9,  V2 = S6 объединение S9,  V3 = S6 \\ S9.");
            MySet<int> s9 = new MySet<int>() { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29 };
            MySet<int> v1 = s6.Intersection(s9);
            Console.Write("v1: ");
            foreach (int i in v1)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            MySet<int> v2 = s6 + s9;
            Console.Write("v2: ");
            foreach (int i in v2)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();
            MySet<int> v3 = s6 / s9;
            Console.Write("v3: ");
            foreach (int i in v3)
                Console.Write(i.ToString() + " ");
            Console.WriteLine();

            //9) Измените V1, объединив его с V3.Сравните V1  с S6.
            Console.WriteLine("9) Измените V1, объединив его с V3.Сравните V1  с S6.");
            v1 = v1 + v3;
            if (v1 == s6)
                Console.WriteLine("Множества идентичны");
            else
                Console.WriteLine("Множества не идентичны");

            //10) Измените множество V2, заменив его разностью V2 и V3. Сравните V2  с S9
            Console.WriteLine("10) Измените множество V2, заменив его разностью V2 и V3. Сравните V2  с S9");
            v2 = v2 / v3;
            if (v2 == s9)
                Console.WriteLine("Множества идентичны");
            else
                Console.WriteLine("Множества не идентичны");


            
            //Console.WriteLine("Сборщик мусора запущен");
            //System.GC.Collect();

        }
    }


    public class MyList<T> : IEnumerable
    {
        public class Cell<T>
        {
            private T _cont;
            private Cell<T> _next = null;

            public Cell(T value)
            {
                _cont = value;
            }
            public Cell() { }
            public Cell<T> Next()
            {
                return _next;
            }
            public void SetNext(Cell<T> n)
            {
                _next = n;
            }
            public ref T GetT()
            {
                return ref _cont;
            }
        }

        protected Cell<T> _first = null;

        //1)	конструкторы
        public MyList(T[] arr)
        {
            _first = new Cell<T>(arr[0]);
            Cell<T> curr = _first;
            for (int i = 1; i < arr.Length; i++)
            {
                curr.SetNext(new Cell<T>(arr[i]));
                curr = curr.Next();
            }
        }
        public MyList() { }
        //перечисления
        private class MyEnumerator : IEnumerator
        {
            Cell<T> _first = null;
            Cell<T> _current = null;

            public MyEnumerator(Cell<T> list)
            {
                _first = list;
            }
            public bool MoveNext()
            {
                if (_current == null)
                {
                    _current = _first;
                    return _current != null;
                }

                if (_current.Next() != null)
                {
                    _current = _current.Next();
                    return true;
                }
                return false;
            }
            public void Reset()
            {
                _current = null;
            }
            public object Current
            {
                get { return _current.GetT(); }
            }
        }
        public IEnumerator GetEnumerator()
        {
            return new MyEnumerator(_first);
        }

        // //2)	Node* find(int) (на входе – ключ, на выходе – указатель на элемент (если нашли), NULL (если нет)),
        public Cell<T> Find(T key)
        {
            Cell<T> curr = _first;
            while (curr != null && !curr.GetT().Equals(key))
                curr = curr.Next();
            return curr;
        }


        // //4)	сортировка,
        public void Sort(bool reverse = false)
        {
            Cell<T> curr = _first;
            Cell<T> min = null;
            Cell<T> begin = null;
            IComparable minV = _first.GetT() as IComparable;
            while (curr != null && curr.Next() != null)
            {
                while (curr.Next() != null)
                {
                    IComparable currV = curr.Next().GetT() as IComparable;
                    if (currV == null || minV == null)
                        throw new NotSupportedException();
                    if (minV.CompareTo(currV) == 1)
                    {
                        min = curr;
                        minV = currV;
                    }
                    curr = curr.Next();
                }
                if (begin == null)
                {
                    if (min != null)
                    {
                        curr = min.Next();
                        min.SetNext(min.Next().Next());
                        curr.SetNext(_first);
                        _first = curr;
                    }
                    begin = _first;
                }
                else
                {
                    curr = min.Next();
                    min.SetNext(min.Next().Next());
                    curr.SetNext(begin.Next());
                    begin.SetNext(curr);
                    begin = curr;
                }
                min = begin;
                curr = min.Next();
                minV = min.Next().GetT() as IComparable;
            }
            if (reverse)
                Reverse();
        }

        // //5)	добавление элемента в голову, хвост, на позицию, после ключа,
        public void Push_front(T elem)
        {
            Cell<T> toAdd = new Cell<T>(elem);
            toAdd.SetNext(_first);
            _first = toAdd;
        }
        public void Push_back(T elem)
        {
            if (_first == null)
            {
                _first = new Cell<T>(elem);
            }
            else
            {
                Cell<T> curr = _first;
                while (curr.Next() != null)
                    curr = curr.Next();
                curr.SetNext(new Cell<T>(elem));
            }
        }
        public void Insert(int index, T value)
        {
            if (index < 0)
            {
                Console.WriteLine("List index out of range");
                Environment.Exit(0);
            }
            Cell<T> toInsert = new Cell<T>(value);
            if (index == 0)
            {
                toInsert.SetNext(_first);
                _first = toInsert;
            }
            else
            {
                Cell<T> curr = _first;
                int i = 1;
                while (i < index && curr.Next() != null)
                {
                    curr = curr.Next();
                    i++;
                }
                toInsert.SetNext(curr.Next());
                curr.SetNext(toInsert);
            }

        }
        public void InsertAfter(T pos, T value)
        {
            Cell<T> curr = _first;
            while (curr != null && !curr.GetT().Equals(pos))
                curr = curr.Next();
            if (curr != null)
            {
                Cell<T> added = new Cell<T>(value);
                added.SetNext(curr.Next());
                curr.SetNext(added);
            }
        }

        // //6)	удаление элемента из головы, хвоста, позиции, по ключу, 
        public void Pop_front()
        {
            if (_first != null)
            {
                T toRet = _first.GetT();
                _first = _first.Next();
            }
        }
        public void Pop_back()
        {
            if (_first != null)
            {
                if (_first.Next() == null)
                {
                    _first = null;
                }
                else
                {
                    Cell<T> curr = _first;
                    while (curr.Next().Next() != null)
                        curr = curr.Next();
                    curr.SetNext(null);
                }
            }
        }
        public void Pop(int index)
        {
            if (index < 0)
            {
                Console.WriteLine("List index out of range");
                Environment.Exit(0);
            }
            if (index == 0)
                _first = _first.Next();
            else
            {
                Cell<T> curr = _first;
                int i = 1;
                while (i < index && curr.Next().Next() != null)
                {
                    curr = curr.Next();
                    i++;
                }
                curr.SetNext(curr.Next().Next());
            }
        }
        public void Remove(T value)
        {
            if (_first != null && _first.GetT().Equals(value))
            {
                _first = _first.Next();
            }
            else
            {
                Cell<T> curr = _first;
                while (curr != null && curr.Next() != null && !curr.Next().GetT().Equals(value))
                    curr = curr.Next();
                if (curr != null && curr.Next() != null)
                    curr.SetNext(curr.Next().Next());
            }
        }

        // //7)	max, min,

        public T Max()
        {
            IComparable ans = _first.GetT() as IComparable;
            T toRet = _first.GetT();
            if (ans == null)
                throw new NotSupportedException();
            Cell<T> curr = _first.Next();
            while (curr != null)
            {
                IComparable a = curr.GetT() as IComparable;
                if (a == null)
                    throw new NotSupportedException();
                if (ans.CompareTo(a) == -1)
                {
                    ans = curr.GetT() as IComparable;
                    if (ans == null)
                        throw new NotSupportedException();
                    toRet = curr.GetT();
                }
                curr = curr.Next();
            }

            return toRet;
        }
        public T Min()
        {
            IComparable ans = _first.GetT() as IComparable;
            T toRet = _first.GetT();
            if (ans == null)
                throw new NotSupportedException();
            Cell<T> curr = _first.Next();
            while (curr != null)
            {
                IComparable a = curr.GetT() as IComparable;
                if (a == null)
                    throw new NotSupportedException();
                if (ans.CompareTo(a) == 1)
                {
                    ans = curr.GetT() as IComparable;
                    if (ans == null)
                        throw new NotSupportedException();
                    toRet = curr.GetT();
                }
                curr = curr.Next();
            }

            return toRet;
        }


        // //8)	Проверка пустоты списка, очистка списка
        public bool Empty()
        {
            return _first == null;
        }
        public void Clear()
        {
            _first = null;
        }

        //	перегрузка операторов: 



        // //10)	[ ] , 
        public Cell<T> FindByIndex(int index)
        {
            if (index < 0)
            {
                return null;
            }
            Cell<T> curr = _first;
            int i = 0;
            while (i < index && curr != null)
            {
                curr = curr.Next();
                i++;
            }
            return curr;
        }
        public T this[int index]
        {
            get => FindByIndex(index).GetT();
            set => FindByIndex(index).GetT() = value;
        }

        // //11)	+(List),  (в голову, в хвост)
        public static MyList<T> operator +(MyList<T> first, MyList<T> second)
        {
            MyList<T> f = first.Copy();
            MyList<T> s = second.Copy();

            Cell<T> curr = f._first;
            while (curr.Next() != null)
                curr = curr.Next();
            curr.SetNext(s._first);
            return f;
        }

        // //12)	== (сравнение), 
        public static bool operator ==(MyList<T> first, MyList<T> second)
        {
            Cell<T> f = first._first;
            Cell<T> s = second._first;
            while (f != null && s != null)
            {
                if (!f.GetT().Equals(s.GetT()))
                    return false;
                f = f.Next();
                s = s.Next();
            }
            if (f != s)
                return false;
            return true;
        }
        public static bool operator !=(MyList<T> first, MyList<T> second)
        {
            return !(first == second);
        }


        /////////

        public MyList<T> Copy()
        {
            MyList<T> copy = new MyList<T>();
            if (_first != null)
            {
                copy._first = new Cell<T>(_first.GetT());
                Cell<T> curr = _first;
                Cell<T> cCurr = copy._first;
                while (curr.Next() != null)
                {
                    cCurr.SetNext(new Cell<T>(curr.Next().GetT()));
                    curr = curr.Next();
                    cCurr = cCurr.Next();
                }
            }
            return copy;
        }
        public void Reverse()
        {
            if (_first != null && _first.Next() != null)
            {
                Cell<T> save = _first.Next().Next();
                Cell<T> toAdd = _first.Next();
                _first.SetNext(null);

                while (toAdd != null)
                {
                    toAdd.SetNext(_first);
                    _first = toAdd;
                    toAdd = save;
                    if (save != null)
                        save = save.Next();
                }
            }
        }
        public ref T Front()
        {
            return ref _first.GetT();
        }
        public ref T Back()
        {
            Cell<T> curr = _first;
            while (curr.Next() != null)
                curr = curr.Next();
            return ref curr.GetT();
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as MyList<T>);
        }
        public bool Equals(MyList<T> other)
        {
            return other == this;
        }

        public virtual void Add(T val)
        {
            Push_back(val);
        }
    }

    public class MySet<T> : MyList<T>
    {
        private int _size = 0;

        // //1) конструкторы
        public MySet(MyList<T> list)
        {
            if (!list.Empty())
            {
                _first = new Cell<T>(list[0]);
                Cell<T> curr = _first;
                Cell<T> back = _first;
                _size++;
                foreach (T t in list)
                {
                    bool found = false;
                    while (curr != null)
                    {
                        if (curr.GetT().Equals(t))
                        {
                            found = true;
                            break;
                        }
                        curr = curr.Next();
                    }
                    if (!found)
                    {
                        back.SetNext(new Cell<T>(t));
                        back = back.Next();
                        _size++;
                        curr = _first;
                    }
                }
            }

        }

        public MySet() { }
        // //3)	поиск,
        public bool Contains(T val)
        {
            Cell<T> curr = _first;
            while (curr != null)
            {
                if (curr.GetT().Equals(val))
                    return true;
                curr = curr.Next();
            }
            return false;
        }

        //перегрузка операторов: 
        // //6)	== (сравнение), 
        public static bool operator ==(MySet<T> first, MySet<T> second)
        {
            MySet<T> f = first.Copy();
            MySet<T> s = second.Copy();
            f.Sort();
            s.Sort();
            return (f as MyList<T>) == (s as MyList<T>);
        }

        public static bool operator !=(MySet<T> first, MySet<T> second)
        {
            return !(first == second);
        }
        // //7)	объединение двух множеств, 
        public static MySet<T> operator +(MySet<T> first, MySet<T> second)
        {
            MySet<T> toRet = first.Copy();
            foreach (T t in second)
                toRet = toRet + t;
            return toRet;
        }
        // //8)	добавление элемента к множеству(создается новое множество, изменяется исходное множество)
        public static MySet<T> operator +(MySet<T> set, T val)
        {
            MySet<T> toRet = set.Copy();
            if (!toRet.Contains(val))
            {
                toRet.Push_front(val);
                toRet._size++;
            }
            return toRet;
        }
        // //9)	пересечение двух множеств(создается новое множество, изменяется исходное множество)
        public MySet<T> Intersection(MySet<T> second)
        {
            MySet<T> toRet = new MySet<T>();
            foreach (T t in second)
                if (Contains(t))
                    toRet.Push_front(t);
            return toRet;
        }
        // //10)	разность двух множеств(создается новое множество, изменяется исходное множество)
        public static MySet<T> operator /(MySet<T> first, MySet<T> second)
        {
            MySet<T> toRet = first.Copy();
            foreach (T t in second)
                toRet = toRet / t;
            return toRet;
        }

        // //13)	удаление элемента из множества(создается новое множество, изменяется исходное множество)
        public static MySet<T> operator /(MySet<T> set, T val)
        {
            MySet<T> toRet = set.Copy();
            if (toRet.Contains(val))
            {
                toRet.Remove(val);
                toRet._size--;
            }
            return toRet;
        }
        /////////
        public override void Add(T val)
        {
            if (!Contains(val))
                Push_front(val);
            _size++;
        }
        public int Size()
        {
            return _size;
        }

        public new MySet<T> Copy()
        {
            MySet<T> copy = new MySet<T>();
            copy._size = _size;
            if (_first != null)
            {
                copy._first = new Cell<T>(_first.GetT());
                Cell<T> curr = _first;
                Cell<T> cCurr = copy._first;
                while (curr.Next() != null)
                {
                    cCurr.SetNext(new Cell<T>(curr.Next().GetT()));
                    curr = curr.Next();
                    cCurr = cCurr.Next();
                }
            }
            return copy;
        }

    }
}
