#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<cmath>
#include<cstdlib>

using namespace std;

template<class T>
class Term {
public:
    int row, col;
    T value;
};


template<class T>
class SparseMatrix {
//    friend ostream&operator<<(ostream&,const SparseMatrix<T>&);
//    friend istream&operator>>(istream&,SparseMatrix);

public:
    SparseMatrix(int maxTerms = 10);

    ~SparseMatrix() { delete[]a; }

    void Append(const Term<T> &t);

    void Add(const SparseMatrix<T> &b, SparseMatrix<T> &c) const;

    void Minus(const SparseMatrix<T> &m, SparseMatrix<T> &n) const;

//private:
    //void Append{const Term<T>&t};
    int rows, cols;
    //����ά��
    int terms;
    //����Ԫ����Ŀ
    Term<T> *a;
    //�洢����Ԫ�ص�����
    int MaxTerms;//����a�Ĵ�С
};

template<class T>
SparseMatrix<T>::SparseMatrix(int maxTerms) {//ϡ������캯��
    //if(maxTerms<1) throw Badlnitializers();
    MaxTerms = maxTerms;
    a = new Term<T>[MaxTerms];
    terms = rows = cols = 0;
};

template<class T>
void SparseMatrix<T>::Append(const Term<T> &t) {
    a[terms] = t;
    terms++;
}


//����<<
template<class T>
ostream &operator<<(ostream &out, const SparseMatrix<T> &x) {//��*this���������
    //������������
    out << "rows=" << x.rows << "columns=" << x.cols << endl;
    out << "nonzero terms" << x.terms << endl;
    //������������ÿ��һ��
    for (int i = 0; i < x.terms; i++)
        out << "a(" << x.a[i].row << ',' << x.a[i].col << ")=" << x.a[i].value << endl;
    return out;
}

//����>>
template<class T>
istream &operator>>(istream &in, SparseMatrix<T> &x) {//����һ��ϡ�����
    //������������
    cout << "Enter number of rows,columns,and terms" << endl;
    in >> x.rows >> x.cols >> x.terms;
    //if(x.term>x.MaxTerms) throw NoMem();
    //�������Ԫ��
    for (int i = 0; i < x.terms; i++) {
        cout << "Enter row,column,and value of term" << (i + 1) << endl;
        in >> x.a[i].row >> x.a[i].col >> x.a[i].value;
    }
    return in;
}

//����ϡ��������
template<class T>
void SparseMatrix<T>::Add(const SparseMatrix<T> &b, SparseMatrix<T> &c) const {//����c=��*this��+b
    //���ý������c������
    c.rows = rows;
    c.cols = cols;
    c.terms = 0;//��ֵ
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
    //����ʣ��Ԫ��
    for (; ct < terms; ct++)
        c.Append(a[ct]);
    for (; cb < b.terms; cb++)
        c.Append(b.a[cb]);
}


//����ϡ��������
template<class T>
void SparseMatrix<T>::Minus(const SparseMatrix<T> &b, SparseMatrix<T> &c) const {//����c=��*this��+b
    //���ý������c������
    c.rows = rows;
    c.cols = cols;
    c.terms = 0;//��ֵ
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
                    t.value = a[ct].value - b.a[ct].value;
                    c.Append(t);
                }
                ct++;
                cb++;
            }
            else {
                b.a[cb].value = -b.a[cb].value;
                c.Append(b.a[cb]);
                b.a[cb].value = -b.a[cb].value;
                cb++;
            }//b����һ��Ԫ��
        }
    }
    //����ʣ��Ԫ��
    for (; ct < terms; ct++)
        c.Append(a[ct]);
    for (; cb < b.terms; cb++)
        c.Append(b.a[cb]);
}



int main() {
    SparseMatrix<int> s1, s2, s3, s4, s;
    cin >> s1;
    cout << "�������" << endl << s1;
    cin >> s2;
    cout << "�������" << endl << s2;
    s1.Add(s2, s3);
    cout << "�����" << endl << s3;
    s1.Minus(s2, s4);
    cout << "�����" << endl << s4;
}
