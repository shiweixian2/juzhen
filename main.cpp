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
    //矩阵维数
    int terms;
    //非零元素数目
    Term<T> *a;
    //存储非零元素的数组
    int MaxTerms;//数组a的大小
};

template<class T>
SparseMatrix<T>::SparseMatrix(int maxTerms) {//稀疏矩阵构造函数
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


//重载<<
template<class T>
ostream &operator<<(ostream &out, const SparseMatrix<T> &x) {//把*this送至输出流
    //输出矩阵的特征
    out << "rows=" << x.rows << "columns=" << x.cols << endl;
    out << "nonzero terms" << x.terms << endl;
    //输出非零个数，每行一个
    for (int i = 0; i < x.terms; i++)
        out << "a(" << x.a[i].row << ',' << x.a[i].col << ")=" << x.a[i].value << endl;
    return out;
}

//重载>>
template<class T>
istream &operator>>(istream &in, SparseMatrix<T> &x) {//输入一个稀疏矩阵
    //输入矩阵的特征
    cout << "Enter number of rows,columns,and terms" << endl;
    in >> x.rows >> x.cols >> x.terms;
    //if(x.term>x.MaxTerms) throw NoMem();
    //输入矩阵元素
    for (int i = 0; i < x.terms; i++) {
        cout << "Enter row,column,and value of term" << (i + 1) << endl;
        in >> x.a[i].row >> x.a[i].col >> x.a[i].value;
    }
    return in;
}

//两个稀疏矩阵相加
template<class T>
void SparseMatrix<T>::Add(const SparseMatrix<T> &b, SparseMatrix<T> &c) const {//计算c=（*this）+b
    //设置结果矩阵c的特征
    c.rows = rows;
    c.cols = cols;
    c.terms = 0;//初值
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
    //复制剩余元素
    for (; ct < terms; ct++)
        c.Append(a[ct]);
    for (; cb < b.terms; cb++)
        c.Append(b.a[cb]);
}


//两个稀疏矩阵相减
template<class T>
void SparseMatrix<T>::Minus(const SparseMatrix<T> &b, SparseMatrix<T> &c) const {//计算c=（*this）+b
    //设置结果矩阵c的特征
    c.rows = rows;
    c.cols = cols;
    c.terms = 0;//初值
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
            }//b的下一个元素
        }
    }
    //复制剩余元素
    for (; ct < terms; ct++)
        c.Append(a[ct]);
    for (; cb < b.terms; cb++)
        c.Append(b.a[cb]);
}



int main() {
    SparseMatrix<int> s1, s2, s3, s4, s;
    cin >> s1;
    cout << "输出矩阵" << endl << s1;
    cin >> s2;
    cout << "输出矩阵" << endl << s2;
    s1.Add(s2, s3);
    cout << "矩阵和" << endl << s3;
    s1.Minus(s2, s4);
    cout << "矩阵差" << endl << s4;
}
