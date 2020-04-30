#include <bits/stdc++.h>
using namespace std;

ifstream f("data.in");
ofstream g("data.out");

class robert {
private:

    static const int sigma = 27;
    std::string tip;
    int nicio_stare, nicio_transformare, nicio_stare_finala, stare_initiala;
    std::unordered_set <int> stari_finale;

    struct nod
    {
        std::unordered_set <int> Tranzitie[sigma];
        ~nod()
        {
            for (int i = 0; i < sigma; ++i)
                Tranzitie[i].clear();
        }
        void operator = (const nod& obiect)
        {
            for (int i = 0; i < sigma; ++i)
                Tranzitie[i] = obiect.Tranzitie[i];
        }
    };

    std::vector <nod> R;

    static inline std::string GetString(const int &x)
    {
        return (x == 26 ? "lambda" : std::string(1, char(x + 'a')));
    }

    static inline char GetCharacter(const std::string &s)
    {
        return (s == "lambda" ? char('z' + 1) : s[0]);
    }

    void LambdaNFA_in_NFA();
    void NFA_in_DFA();
    void DFA_in_MinDFA();

public:

    inline void adaugare_margine(const int &nod1, const int &nod2, const std::string &ch)
    {
        R[nod1].Tranzitie[GetCharacter(ch) - 'a'].emplace(nod2);
    }

    friend std::istream& operator >> (std::istream &in, robert &obiect);

    friend std::ostream& operator << (std::ostream &out, const robert &obiect);

    void transformare();
};

void robert::LambdaNFA_in_NFA()
{
    tip = "NFA";
    bool tranzitie_noua = true;

    while(tranzitie_noua)
    {
        tranzitie_noua = false;

        for(int i = 0; i < nicio_stare; ++i)
        {
            for(auto& next: R[i].Tranzitie[sigma - 1])
            {
                for(auto& it : R[next].Tranzitie[sigma - 1])
                    if(R[i].Tranzitie[sigma - 1].find(it) == R[i].Tranzitie[sigma - 1].end())
                    {
                        tranzitie_noua = true;
                        R[i].Tranzitie[sigma - 1].emplace(it);
                    }
            }
        }
    }

    for(int i = 0; i < nicio_stare; ++i)
    {
        for(auto& next : R[i].Tranzitie[sigma - 1])
        {
            for(int litera = 0; litera < sigma - 1; ++litera)
                for(auto& it : R[next].Tranzitie[litera])
                    R[i].Tranzitie[litera].emplace(it);

            if(stari_finale.find(next) != stari_finale.end())
                stari_finale.emplace(i);
        }

        R[i].Tranzitie[sigma - 1].clear();
    }

    nicio_stare_finala = stari_finale.size();
    nicio_transformare = 0;

    for(int i = 0; i < nicio_stare; ++i)
        for(int j = 0; j < sigma; ++j)
            nicio_transformare += R[i].Tranzitie[j].size();
}

void robert::NFA_in_DFA()
{
    int N_nicio_stare = 0, N_nicio_transformare = 0;

    std::map < std::vector <int>, int> N_stari;
    std::vector <nod> N_R;
    std::unordered_set <int> N_stari_finale;

    N_stari[ {stare_initiala}] = N_nicio_stare++;
    if(stari_finale.find(stare_initiala) != stari_finale.end())
        N_stari_finale.emplace(N_nicio_stare - 1);

    std::queue <std::vector <int> > v;
    std::vector <int> varf, k;
    std::unordered_set <int> k_set;
    v.push({stare_initiala});

    while(!v.empty())
    {
        varf = v.front();
        v.pop();

        for(int litera = 0; litera < sigma; ++litera)
        {
            k.clear();
            k_set.clear();

            for(auto& nanu : varf)
                for(auto& next : R[nanu].Tranzitie[litera])
                {
                    if(k_set.find(next) == k_set.end())
                    {
                        k_set.emplace(next);
                        k.push_back(next);
                    }
                }

            if(k.empty())
                continue;

            sort(k.begin(), k.end());

            if(N_stari.find(k) == N_stari.end())
            {
                N_stari[k] = N_nicio_stare++;
                bool final = false;
                for(auto& nanu : k)
                    if(stari_finale.find(nanu) != stari_finale.end())
                    {
                        final = true;
                        break;
                    }
                if(final)
                    N_stari_finale.emplace(N_nicio_stare - 1);

                v.push(k);
            }

            ++N_nicio_transformare;
            int pozitie = N_stari[varf];
            if(pozitie >= N_R.size())
                N_R.resize(pozitie + 1);
            N_R[pozitie].Tranzitie[litera].emplace(N_stari[k]);
        }
    }

    tip = "DFA";
    nicio_stare = N_nicio_stare;
    nicio_transformare = N_nicio_transformare;
    stare_initiala = 0;
    stari_finale.clear();
    stari_finale = N_stari_finale;
    N_stari_finale.clear();
    nicio_stare_finala = stari_finale.size();
    R.clear();
    R = N_R;
    N_R.clear();
}

std::vector < std::vector <int> > rob;
std::unordered_set <int> aici;
std::vector <int> root;

bool BFS(const int &criteriu_nod, int &N_nicio_stare, const std::unordered_set <int> &stari_finale)
{
    if(aici.find(criteriu_nod) != aici.end())
        return 0;

    ++N_nicio_stare;
    bool raspuns = false;
    std::queue <int> v;
    v.push(criteriu_nod);
    aici.emplace(criteriu_nod);

    while(!v.empty())
    {
        int varf = v.front();
        v.pop();
        root[varf] = N_nicio_stare - 1;
        if(stari_finale.find(varf) != stari_finale.end())
            raspuns = true;

        for(auto& next : rob[varf])
            if(aici.find(next) == aici.end())
            {
                aici.emplace(next);
                v.push(next);
            }
    }

    return raspuns;
}

void robert::DFA_in_MinDFA()
{
    int N_nicio_stare = 0, N_nicio_transformare = 0;
    std::unordered_set <int> N_stari_finale;
    std::vector <nod> N_R;
    std::vector <nod> Rn(nicio_stare);

    for(int i = 0; i < nicio_stare; ++i)
        for(int litera = 0; litera < sigma; ++litera)
        {
            for(auto& next : R[i].Tranzitie[litera])
                Rn[next].Tranzitie[litera].emplace(i);
        }

    std::unordered_set <int> aici1, aici2;
    std::queue <int> v;

    v.push(stare_initiala);
    aici1.emplace(stare_initiala);
    while (!v.empty())
    {
        int varf = v.front();
        v.pop();
        for (int litera = 0; litera < sigma; ++litera)
        {
            for (auto& next : R[varf].Tranzitie[litera])
                if (aici1.find(next) == aici1.end())
                {
                    aici1.emplace(next);
                    v.push(next);
                }
        }
    }

    for(auto& st : stari_finale)
    {
        v.push(st);
        aici2.emplace(st);
    }
    while(!v.empty())
    {
        int varf = v.front();
        v.pop();
        for(int litera = 0; litera < sigma; ++litera)
        {
            for(auto& next : Rn[varf].Tranzitie[litera])
                if(aici2.find(next) == aici2.end())
                {
                    aici2.emplace(next);
                    v.push(next);
                }
        }
    }

    bool null_DFA = false;
    if(aici2.find(stare_initiala) == aici2.end())
        null_DFA = true;

    int cnt = 0;

    for(auto& st : stari_finale)
        if (aici1.find(st) != aici1.end())
            ++cnt;

    if(cnt == 0)
        null_DFA = true;

    if(null_DFA)
    {
        nicio_stare = nicio_transformare = nicio_stare_finala = 0;
        stare_initiala = -1;
        stari_finale.clear();
        R.clear();
        tip = "MinDFA";
        Rn.clear();
        aici1.clear();
        aici2.clear();
        return;
    }

    std::vector <int> N_stare_coresp(nicio_stare, 0);
    for(int i = 0; i < nicio_stare; ++i)
        if(aici1.find(i) == aici1.end() or aici2.find(i) == aici2.end())
            N_stare_coresp[i] = -1;

    Rn.clear();
    aici1.clear();
    aici2.clear();

    N_nicio_stare = nicio_stare;

    int coresp_actual = -1;
    for(int i = 0; i < nicio_stare; ++i)
        if(N_stare_coresp[i] != -1)
            N_stare_coresp[i] = ++coresp_actual;
        else
            --N_nicio_stare;

    N_R.resize(N_nicio_stare);
    for(int i = 0; i < nicio_stare; ++i)
    {
        if(N_stare_coresp[i] == -1)
            continue;

        if(stari_finale.find(i) != stari_finale.end())
            N_stari_finale.emplace(i);

        for(int litera = 0; litera < sigma; ++litera)
            for(auto& next : R[i].Tranzitie[litera])
            {
                if(N_stare_coresp[next] == -1)
                    continue;
                ++N_nicio_transformare;
                N_R[N_stare_coresp[i]].Tranzitie[litera].emplace(N_stare_coresp[next]);
            }
    }

    nicio_stare = N_nicio_stare;
    nicio_transformare = N_nicio_transformare;
    stare_initiala = N_stare_coresp[stare_initiala];
    stari_finale.clear();
    stari_finale = N_stari_finale;
    N_stari_finale.clear();
    nicio_stare_finala = stari_finale.size();
    R.clear();
    R = N_R;
    N_R.clear();
    N_stare_coresp.clear();
    N_nicio_transformare = N_nicio_stare = 0;

    std::set < std::pair <int, int> > ramas;
    for(int i = 0; i < nicio_stare; ++i)
        for(int j = i + 1; j < nicio_stare; ++j)
        {
            bool _ = (bool)(stari_finale.find(i) != stari_finale.end());
            bool __ = (bool)(stari_finale.find(j) != stari_finale.end());
            if (!(_ ^ __)) ramas.emplace(std::make_pair(i, j));
        }

    bool N_marcare = true;
    while(N_marcare)
    {
        N_marcare = false;
        std::vector < std::pair <int, int> > k;
        for(auto& p : ramas)
            for(int litera = 0; litera < sigma; ++litera)
            {
                if(R[p.first].Tranzitie[litera].empty() or R[p.second].Tranzitie[litera].empty())
                    continue;
                int a = *R[p.first].Tranzitie[litera].begin(), b = *R[p.second].Tranzitie[litera].begin();
                if(a > b)
                    std::swap(a, b);

                if (ramas.find(std::make_pair(a, b)) == ramas.end())
                {
                    k.push_back(p);
                    break;
                }
            }

        if (!k.empty()) N_marcare = true;
        for (auto& it : k)
            ramas.erase(it);
        k.clear();
    }

    rob.resize(nicio_stare);
    for(auto& it : ramas)
    {
        rob[it.first].push_back(it.second);
        rob[it.second].push_back(it.first);
    }

    root.resize(nicio_stare);
    for(int i = 0; i < nicio_stare; ++i)
        root[i] = i;

    for(int i = 0; i < nicio_stare; ++i)
    {
        bool final = BFS(i, N_nicio_stare, stari_finale);
        if(final)
            N_stari_finale.emplace(N_nicio_stare - 1);
    }

    N_R.resize(N_nicio_stare);
    for(int i = 0; i < nicio_stare; ++i)
        for(int litera = 0; litera < sigma; ++litera)
        {
            if(R[i].Tranzitie[litera].empty())
                continue;
            int from = root[i], to = root[*R[i].Tranzitie[litera].begin()];

            if(!N_R[from].Tranzitie[litera].empty())
                continue;
            ++N_nicio_transformare;
            N_R[from].Tranzitie[litera].emplace(to);
        }

    tip = "MinDFA";
    nicio_stare = N_nicio_stare;
    nicio_transformare = N_nicio_transformare;
    stare_initiala = root[stare_initiala];
    stari_finale.clear();
    stari_finale = N_stari_finale;
    N_stari_finale.clear();
    nicio_stare_finala = stari_finale.size();
    R.clear();
    R = N_R;
    N_R.clear();
    rob.clear();
    root.clear();
    ramas.clear();
    aici.clear();
}

void robert::transformare()
{
    LambdaNFA_in_NFA();
    NFA_in_DFA();
    DFA_in_MinDFA();
}

std::istream& operator >> (std::istream &in, robert &obiect)
{
    in >> obiect.tip;
    in >> obiect.nicio_stare >> obiect.nicio_transformare;
    obiect.R.resize(obiect.nicio_stare);

    for (int nanu = 0; nanu < obiect.nicio_transformare; ++nanu)
    {
        int x, y;
        std::string str;
        in >> x >> y >> str;
        obiect.adaugare_margine(x, y, str);
    }

    in >> obiect.stare_initiala;

    in >> obiect.nicio_stare_finala;
    for (int nanu = 0; nanu < obiect.nicio_stare_finala; ++nanu)
    {
        int x;
        in >> x;
        obiect.stari_finale.emplace(x);
    }

    return in;
}

std::ostream& operator << (std::ostream &out, const robert &obiect)
{
    out << "tip = " << obiect.tip << '\n';
    out << obiect.nicio_stare << " stari\n";
    out << obiect.nicio_transformare << " tranzitii\n";

    out << "Tranzitiile sunt:\n";
    for (int nanu = 0; nanu < obiect.nicio_stare; ++nanu)
        for (int i = 0; i < robert::sigma; ++i)
            for (auto& it : obiect.R[nanu].Tranzitie[i])
                out << nanu << ' ' << it << ' ' << robert::GetString(i) << '\n';

    out << "Stare initiala: " << obiect.stare_initiala << '\n';

    out << obiect.nicio_stare_finala << " stari finale:\n";
    for (auto& it : obiect.stari_finale)
        out << it << ' ' << '\n';

    return out;
}

//##########################################################################################

int main()
{
    robert D;
    f >> D;

    D.transformare();

    g << D;
    return 0;
}
