#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>


using namespace std;


struct Case
{
    bool filled = false;
    int count_prods = 0;
    int x;
    int y;
    int z;
    int id;
    list<int> prodsInCaseID;
    Case() {}
    Case(int _id, int _x, int _y, int _z) : id(_id), x(_x), y(_y), z(_z) {}
};

struct Product {
    bool packed = false;
    int x;
    int y;
    int z;
    int id;
    Product() {}
    Product(int _id, int _x, int _y, int _z) : id(_id), x(_x), y(_y), z(_z) {}
};


vector <Product> notFitProds;

int gRN(int min, int max);
template <typename T>
void FillRects(vector<T>& R, int N, int a = 1, int b = 40);
bool WriteOutputDataOrderline(const char* NameFile, const vector<Case>& cases);

void sort_algh_FFDH(vector<Product>& vP, vector<Case>& vC) {
    sort(vP.begin(), vP.end(), [](Product& a, Product& b) { return a.z > b.z; });
    sort(vC.begin(), vC.end(), [](Case& a, Case& b) { return a.z < b.z; });

    bool all_cases = true;
    bool all_prods = true;
    vector<Product>::iterator P;


    for (auto itC = vC.begin(); itC != vC.end(); ++itC)
    {
        int count_cases = count_if(vC.begin(), vC.end(), [](Case& a) { return a.filled; });
        if (count_cases == vC.size()) all_cases = false;

        if (!all_cases || !all_prods) { break; }

        // looking for the smallest box that will fit the largest product
        bool findC = false;
        bool en = false;
        for (auto itL = vP.begin(); itL != vP.end(); ++itL)
        {

            if ((!(itL->packed)) && ((itC->z) - (itL->z) >= 0) && ((itC->x) - (itL->x) >= 0) && ((itC->y) - (itL->y) >= 0)) {
                P = itL;
                findC = true;

                break;
            }
        }

        if (!findC) continue;

        int z_floor = 0;
        int z_ceiling = P->z;
        int pVol = 0;
        int cVol = itC->x * itC->y * itC->z;
        bool eW = false;
        while ((z_ceiling <= itC->z) || !eW)
        {
            struct box_prop {
                int x_l = 0, y_l = 0;
            };
            int lvl = 0;
            int lvl_cur = 1;
            int h_box = 0;
            vector <box_prop> t_lvl;
            box_prop temp;

            h_box = temp.y_l = P->y;
            temp.x_l = P->x;
            P->packed = true;
            itC->prodsInCaseID.push_back(P->id);
            itC->count_prods++;
            pVol += P->x * P->y * P->z;
            t_lvl.push_back(temp);

            if ((P + 1) != vP.end()) {
                bool box_post = false;
                for (auto it = P + 1; it != vP.end(); ++it)
                {
                    if (cVol < pVol) { en = true; break; }
                    if (it->packed || ((itC->x) - (it->x) < 0) || ((itC->y) - (it->y) < 0) || (it->z + z_floor > z_ceiling)) continue;

                    for (int lvl = 0; lvl < lvl_cur; lvl++)
                    {
                        if ((itC->x - (t_lvl.at(lvl).x_l)) >= it->x) {
                            t_lvl.at(lvl).x_l += it->x;
                            it->packed = true;
                            box_post = true;
                            itC->count_prods++;
                            itC->prodsInCaseID.push_back(it->id);
                            pVol += it->x * it->y * it->z;
                            break;
                        }
                    }
                    if ((!box_post) && ((h_box + it->y) <= itC->y) && (it->z + z_floor > z_ceiling)) {
                        lvl_cur++;
                        it->packed = true;
                        itC->prodsInCaseID.push_back(it->id);
                        temp.y_l = it->y;
                        h_box += it->y;
                        temp.x_l = it->x;
                        itC->count_prods++;
                        pVol += it->x * it->y * it->z;
                        t_lvl.push_back(temp);

                    }

                    box_post = false;

                }
            }
            t_lvl.clear();

            vector<Product>::iterator Q;
            bool notFind3dF = false;
            for (auto itQ = P; itQ != vP.end(); ++itQ)
            {
                if (!(itQ->packed) && (z_ceiling + itQ->z <= itC->z) && ((itC->x) - (itQ->x) >= 0) && ((itC->y) - (itQ->y) >= 0)) {
                    Q = itQ;
                    notFind3dF = true;
                    break;
                }
            }

            if (!notFind3dF) { eW = true; break; }
            if (z_ceiling + Q->z > itC->z) { eW = true; break; }
            P = Q;
            z_floor = z_ceiling;
            z_ceiling += P->z;

            int count_prods = count_if(vP.begin(), vP.end(), [](Product& a) { return a.packed; });
            if (count_prods == vP.size()) all_prods = false;
            if (!all_cases || !all_prods) { eW = true;  break; }


        }
        itC->filled = true;
    }
}
bool check_all_cases(vector<Product>& vP, vector<Case>& vC);

vector <Product> products;
vector <Case> cases;


//Set nums products and cases
const int numP = 10;
const int numC = 5000;

void start_norm_rand() {
    srand(static_cast<unsigned int>(time(0)));
    rand();
}

int main()
{

    start_norm_rand();
    FillRects(products, numP);
    FillRects(cases, numC);

    sort_algh_FFDH(products, cases);


    WriteOutputDataOrderline("Orderline.txt", cases);
    return 0;
}


bool check_all_cases(vector<Product>& vP, vector<Case>& vC) {

    for (auto it = vC.begin(); it != vC.end(); ++it)
    {
        int capC = (it->y * it->x * it->z);
        int capP = 0;
        int k = 0;

        for (auto itID = it->prodsInCaseID.begin(); itID != it->prodsInCaseID.end(); ++itID)
        {
            for (auto itP = vP.begin(); itP != vP.end(); ++itP)
            {
                if ((*itID) == itP->id) {
                    capP += (itP->x) * (itP->y) * (itP->z);
                    break;
                }
            }
        }
        double kkd = (((double)capP / capC) * 100);
        if (kkd > 100) return false;
    }
    return true;
}

template <typename T>
void FillRects(vector<T>& R, int N, int a, int b) {
    T temp;
    for (int i = 0; i < N; ++i)
    {
        temp.id = i;
        temp.x = gRN(a, b);
        temp.y = gRN(a, b);
        temp.z = gRN(a, b);
        R.push_back(temp);
    }
}

bool WriteOutputDataOrderline(const char* NameFile, const vector<Case>& cases) {
    ofstream fout(NameFile);
    for (auto it = cases.begin(); it != cases.end(); ++it)
    {
        if (!it->filled) continue;
        fout << "id: " << it->id;
        fout << "\t\tNOP: " << it->count_prods;
        fout << "\t\t\tID products:";
        for (auto i = it->prodsInCaseID.begin(); i != it->prodsInCaseID.end(); ++i)
        {
            fout << " " << *i;
        }
        fout << "\n";
    }
    fout.close();
    return true;
}



int gRN(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}