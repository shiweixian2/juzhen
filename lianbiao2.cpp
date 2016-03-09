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
    //ɾ�����нڵ�
    ChainNode<T> *next;//��һ���ڵ�
    while (first) {
        next = first->link;
        delete first;
        first = next;
    }
}

template<class T>
void Chain<T>::Erase() {
    //ɾ�����нڵ�
    ChainNode<T> *next;//��һ���ڵ�
    while (first) {
        next = first->link;
        delete first;
        first = next;
    }
}

template<class T>
Chain<T> &Chain<T>::Append(const T &x) {
//������β�����x
    ChainNode<T> *y;
    y = new ChainNode<T>;
    y->data = x;
    y->link = 0;
    //����ǿ�
    if (first) {
        last->link = y;
        last = y;
    }
    else//����Ϊ��
        first = last = y;
    return *this;

}

template<class T>
void Chain<T>::Zero() {
    first = 0;
}


//ϡ��������õ�����ڵ�
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
    //������
    HeadNode<T> *link;
};

template<class T>
ostream &operator<<(ostream &out, const HeadNode<T> &x) {
    x.Output(out);
    out << endl;
    return out;
}

//ϡ�������Ķ���
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
    Chain<HeadNode<T> > a;//ͷ�������

};

template<class T>
void LinkedMatrix<T>::Add(const LinkedMatrix<T> &b, LinkedMatrix<T> &c) const {
    //���ý������c������
    c.rows = rows;
    c.cols = cols;
    //����*this��b���α�
    int ct = 0, cb = 0;
    //��this��b�б���
    while (ct < terms && cb < b.terms) {
        //ÿһ��Ԫ�ص���������
        int indt = a[ct].row * cols + a[ct].col;
        int indb = b.a[cb].row * cols + b.a[cb].col;
        if (indt < indb) {
            //b��Ԫ���ں�
            c.Append(a[ct]);
            ct++;
        }//*this����һ��Ԫ��
        else {
            if ((indt == indb)) {
                //λ����ͬ
                //�����Ͳ�Ϊ0ʱ����ӵ�c��
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
            }//b����һ��Ԫ��
        }
}


//���������
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




//����>>����һ��ϡ�����

template<class T>
istream &operator>>(istream &in, LinkedMatrix<T> &x) {//�����������������x
    x.a.Erase();//ɾ��x�����нڵ�
    //��ȡ��������
    int terms;//����Ԫ�صĸ���
    cout << "enter numbers of rows,cols,terms" << endl;
    in >> x.rows >> x.cols >> terms;
    //�����0��
    HeadNode<T> H;//��ǰ�е�ͷ�ڵ�
    H.row = 0;//��ǰ�к�
    //����x�ķ�0Ԫ��
    for (int i = 0; i < terms; i++) {
        //������һ��Ԫ��
        cout << "enter row,column,and the value of term" << i << endl;
        int row, col;
        T value;
        in >> row >> col >> value;
        //�����Ԫ���Ƿ����ڵ�ǰ��
        if (row > H.row) {//��ʼһ������
            //������ǵ�0�У���ѵ�ǰ�е�ͷ���H��ӵ�ͷ�������x.a��
            if (H.row)x.a.Append(H);
            //Ϊ�µ�һ��׼��H ͷ���
            H.row = row;
            H.a.Zero();//������ͷָ��first=0
        }
        //�����Ԫ��
        CNode<T> *c = new CNode<T>;
        c->col = col;
        c->value = value;
        H.a.Append(*c);
    }
    //ע��������һ��
    if (H.row)x.a.Append(H);
    H.a.Zero();//������ͷָ��Ϊ0
    return in;
}

//����<<���һ��ϡ�����

template<class T>
ostream &operator<<(ostream &out, LinkedMatrix<T> &x) {//�Ѿ���x���������
    ChainIterator<HeadNode<T> > p;//ͷ��������
    //��������ά��
    out << "rows=" << x.rows << "columns=" << x.cols << endl;
    //��hָ���һ��ͷ���
    HeadNode<T> *h = p.Initialize(x.a);

    ChainIterator<CNode<T> > q;

    //CNode<T> *a = new CNode<T>;
    if (!h) {
        out << "No non-zero terms" << endl;
        return out;
    }
    //ÿ�����һ��
    while (h) {
        CNode<T> *a = q.Initialize(h->a);
        while (a) {
            out << "rows" << h->row << endl;
            out << "cols" << a->col << endl;
            out << "value" << a->value << endl;
            a = q.Next();
        }

        //out<<"cols"<<a->col<<endl;//���������
        //out<<"value"<<a->value<<endl;
        h = p.Next();//��һ��ͷ���
    }
    return out;
}


int main() {
    int i, j;
    LinkedMatrix<int> s1, s2, s;
    cin >> s1;
    cout << "�������s1:" << endl << s1;
    cin >> s2;
    cout << "�������s2:" << endl << s2;
};

