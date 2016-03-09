#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<cmath>
#include<cstdlib>

using namespace std;

template<class T>
class ChainNode {
//    friend Chain<T>;

public:
    T data;
    ChainNode<T> *link;

};

template<class T>
class Chain {
public:
    Chain() { first = 0; }

    ~Chain();

    void Erase();

    Chain<T> &Append(const T &x);

    void Zero();

//private:
    ChainNode<T> *first;
    ChainNode<T> *last;

};

template<class T>
Chain<T>::~Chain() {
    //删除所有节点
    ChainNode<T> *next;//下一个节点
    while (first) {
        next = first->link;
        delete first;
        first = next;
    }
}

template<class T>
void Chain<T>::Erase() {
    //删除所有节点
    ChainNode<T> *next;//下一个节点
    while (first) {
        next = first->link;
        delete first;
        first = next;
    }
}

template<class T>
Chain<T> &Chain<T>::Append(const T &x) {
//在链表尾部添加x
    ChainNode<T> *y;
    y = new ChainNode<T>;
    y->data = x;
    y->link = 0;
    //链表非空
    if (first) {
        last->link = y;
        last = y;
    }
    else//链表为空
        first = last = y;
    return *this;

}

template<class T>
void Chain<T>::Zero() {
    first = 0;
}


//稀疏矩阵所用的链表节点
template<class T>
class CNode {
public:
    int operator!=(const CNode<T> &y) {
        return (value != y.value);
    }

    void Output(ostream &out) const {
        out << "column:" << col << "value:" << value;
    }

//    private:
    int col;
    T value;
    CNode<T> *link;
};

template<class T>
ostream &operator<<(ostream &out, const CNode<T> &x) {
    x.Output(out);
    out << endl;
    return out;
}

template<class T>
class HeadNode {
public:
    int operator!=(const HeadNode<T> &y) {
        return (row != y.row);
    }

    void Output(ostream &out) const {
        out << "row:" << row;
    }

//    private:
    int row;
    Chain<CNode<T> > a;
    //行链表
    HeadNode<T> *link;
};

template<class T>
ostream &operator<<(ostream &out, const HeadNode<T> &x) {
    x.Output(out);
    out << endl;
    return out;
}

//稀疏矩阵类的定义
template<class T>
class LinkedMatrix {
    //friend ostream&operator<<
    //    (ostream&,const LinkedMatrix<T>&);
    //friend istream&operator<<
    //        (istream&,LinkedMatrix<T>&);
public:
    LinkedMatrix() { };

    ~LinkedMatrix() { }

    void Add(const LinkedMatrix<T> &b, LinkedMatrix<T> &c) const;

    void Minus(const LinkedMatrix<T> &b, LinkedMatrix<T> &c) const;

    T Find(const LinkedMatrix<T> &x, const int r, const int c);

    //   private:
    int rows, cols;
    Chain<HeadNode<T> > a;//头结点链表

};

template<class T>
void LinkedMatrix<T>::Add(const LinkedMatrix<T> &b, LinkedMatrix<T> &c) const {
    //设置结果矩阵c的特征
    c.rows = rows;
    c.cols = cols;
    //定义*this和b的游标
    int ct = 0, cb = 0;
    //在this和b中遍历
    while (ct < terms && cb < b.terms) {
        //每一个元素的行主索引
        int indt = a[ct].row * cols + a[ct].col;
        int indb = b.a[cb].row * cols + b.a[cb].col;
        if (indt < indb) {
            //b的元素在后
            c.Append(a[ct]);
            ct++;
        }//*this的下一个元素
        else {
            if ((indt == indb)) {
                //位置相同
                //仅当和不为0时才添加到c中
                if (a[ct].value + b.a[cb].value) {
                    Term<T> t;
                    t.row = a[ct].row;
                    t.col = a[ct].col;
                    t.value = a[ct].value + b.a[ct].value;
                    c.Append(t);
                }
                ct++;
                cb++;
            }
            else {
                c.Append(b.a[cb]);
                cb++;
            }//b的下一个元素
        }
}


//链表遍历器
template<class T>
class ChainIterator {
public:
    ChainNode<T> *location;

    T *Initialize(const Chain<T> &c) {
        location = c.first;
        if (location) return &location->data;
        return 0;
    }

    T *Next() {
        if (!location) return 0;
        location = location->link;
        if (location) return &location->data;
        return 0;
    }
};




//重载>>输入一个稀疏矩阵

template<class T>
istream &operator>>(istream &in, LinkedMatrix<T> &x) {//从输入流中输入矩阵x
    x.a.Erase();//删除x中所有节点
    //获取矩阵特征
    int terms;//输入元素的个数
    cout << "enter numbers of rows,cols,terms" << endl;
    in >> x.rows >> x.cols >> terms;
    //虚设第0行
    HeadNode<T> H;//当前行的头节点
    H.row = 0;//当前行号
    //输入x的非0元素
    for (int i = 0; i < terms; i++) {
        //输入下一个元素
        cout << "enter row,column,and the value of term" << i << endl;
        int row, col;
        T value;
        in >> row >> col >> value;
        //检查新元素是否属于当前行
        if (row > H.row) {//开始一个新行
            //如果不是第0行，则把当前行的头结点H添加到头结点链表x.a中
            if (H.row)x.a.Append(H);
            //为新的一行准备H 头结点
            H.row = row;
            H.a.Zero();//置链表头指针first=0
        }
        //添加新元素
        CNode<T> *c = new CNode<T>;
        c->col = col;
        c->value = value;
        H.a.Append(*c);
    }
    //注意矩阵最后一行
    if (H.row)x.a.Append(H);
    H.a.Zero();//置链表头指针为0
    return in;
}

//重载<<输出一个稀疏矩阵

template<class T>
ostream &operator<<(ostream &out, LinkedMatrix<T> &x) {//把矩阵x送至输出流
    ChainIterator<HeadNode<T> > p;//头结点遍历器
    //输出矩阵的维数
    out << "rows=" << x.rows << "columns=" << x.cols << endl;
    //将h指向第一个头结点
    HeadNode<T> *h = p.Initialize(x.a);

    ChainIterator<CNode<T> > q;

    //CNode<T> *a = new CNode<T>;
    if (!h) {
        out << "No non-zero terms" << endl;
        return out;
    }
    //每次输出一行
    while (h) {
        CNode<T> *a = q.Initialize(h->a);
        while (a) {
            out << "rows" << h->row << endl;
            out << "cols" << a->col << endl;
            out << "value" << a->value << endl;
            a = q.Next();
        }

        //out<<"cols"<<a->col<<endl;//输出行链表
        //out<<"value"<<a->value<<endl;
        h = p.Next();//下一个头结点
    }
    return out;
}


int main() {
    int i, j;
    LinkedMatrix<int> s1, s2, s;
    cin >> s1;
    cout << "输出矩阵s1:" << endl << s1;
    cin >> s2;
    cout << "输出矩阵s2:" << endl << s2;
};

