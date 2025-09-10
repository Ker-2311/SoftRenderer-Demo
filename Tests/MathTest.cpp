#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "iostream"

using namespace std;

int main() {
    Vector2f va(1.0f,2.0f);
    Vector2f vb(2.0f,1.0f);
    Vector2f vc;
    vc = va + vb;

    cout << "vc:" <<vc.x << "," << vc.y << endl;

    Vector2f vd;
    vd = va + 2;
    cout << "vd:" <<vd.x << "," << vd.y << endl;

    Vector2f ve;
    ve = va * vd;
    cout << "ve:" <<ve.x << "," << ve.y << endl;
    cout << "va dot vb:" << va.Dot(vb) << endl;

    Vector3f va1(1.0f,2.0f,3.0f);
    Vector3f vb1(2.0f,1.0f,1.0f);
    Vector3f vc1;
    vc1 = va1.Cross(vb1);

    cout << "vc1:" <<vc1.x << "," << vc1.y << "," << vc1.z << endl;

    Matrix4x4f m4;
    m4.Identity();
    cout<<"Identity Matrix：" << endl;
    m4.Print();

    Matrix<float,3,2> m1 = {1,2,3,4,5,6};
    Matrix<float,2,3> m2 = {1,2,3,4,5,6};
    cout<<"m1：" << endl;
    m1.Print();
    cout<<"m2：" << endl;
    m2.Print();

    cout<<"m1 * m2：" << endl;
    (m1 * m2).Print(); 
}