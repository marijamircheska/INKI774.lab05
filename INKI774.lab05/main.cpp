#include <ios>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

struct qna {
    string prasanje;
    string odgovor;
    string *odgovori = new string[3]();
};

struct student {
    string ime;
    string prezime;
    string indeks;
};

vector<string> split_string(string s, char ch){
    vector<string> strings;
    string current = "";
    for (signed int i = 0; i < s.size(); i++) {
        if (s[i] == ch) {
            strings.push_back(current);
            current = "";
            continue;
        }
        current += s[i];
    }
    return strings;
}

string read_f(string path) {
    fstream f(path, fstream::in );
    string s;
    getline( f, s, '\0');
    f.close();
    return s;
}

string tp_vo_string(const chrono::system_clock::time_point& tp) {
    time_t t = chrono::system_clock::to_time_t(tp);
    string ts = ctime(&t);
    ts.resize(ts.size()-1);
    return ts;
}

int num = 0;
qna* zemi_parasanja(string prasanja_path) {
    string prasanja_str = read_f(prasanja_path);
    vector<string> prasanja_vector = split_string(prasanja_str, ';');
    qna *ret = new qna[prasanja_vector.size()]();
    num = prasanja_vector.size() / 5;
    for (signed int i = 0; i < prasanja_vector.size(); ++i) {

        if (i % 5 == 0) ret[i / 5].prasanje = prasanja_vector[i];
        else if (i % 5 == 4) ret[i / 5].odgovor = prasanja_vector[i];
        else ret[i / 5].odgovori[(i % 5) - 1] = prasanja_vector[i];
    }
    return ret;
}

void zapishi_vo_log(student S, const chrono::system_clock::time_point& vreme) {
    string log_str = read_f("./Students.log");
    vector<string> prasanja_vector = split_string(log_str, ';');
    int num = prasanja_vector.size() + 1;
    string log = tp_vo_string(vreme) + "; " + S.ime + ", " + S.prezime + ", " + S.indeks;
    ofstream file;
    file.open("./Students.log", ios_base::app);
    file << num << ". " << log << endl;
    file.close();
}

student najava_na_student() {
    student _student;
    cout << "Ime: ";
    getline(cin, _student.ime, '\n');
    cout << "Prezime: ";
    getline(cin, _student.prezime, '\n');
    cout << "Br. Indeks primer:(INKI012): ";
    getline(cin, _student.indeks, '\n');
    string f_name = _student.ime+_student.prezime+_student.indeks;
    ifstream _file(f_name);
    if(_file.is_open()){
        cout << "Nemate pravo povtorno da polagate! " << endl;
        _file.close();
        return {"NA", "NA", "NA"};
    }
    return _student;
}

int main() {
    qna* prasanja = zemi_parasanja("./prasanja");
    student _student = najava_na_student();
    if (_student.ime == "NA") return 0;
    auto pocetok = chrono::system_clock::now();
    string odgovori[num];
    int poeni = 0;
    ofstream file;
    file.open(_student.ime+_student.prezime+_student.indeks);

    file << "Ime: " + _student.ime << endl;
    file << "Prezime: " + _student.prezime << endl;
    file << "Indeks: " + _student.indeks << endl;
    file << endl;
    file << "Vreme na pocetok: " << tp_vo_string(pocetok) << endl;

    for (signed int i = 0; i < num; ++i) {
        cout << prasanja[i].prasanje << endl;
        for (signed int j = 0; j < 3; j++) {
            cout << prasanja[i].odgovori[j] << endl;;
        }
        string odgovor;
        cin >> odgovor;
        while (odgovor != "a" && odgovor != "b" && odgovor != "c") {
            cout << "Vashiot odgovor ne e validen. Vnesete edna od opciite (a, b, c)";
            cin >> odgovor;
        }
        odgovori[i] = odgovor;
        if (odgovor == prasanja[i].odgovor) {
            poeni+= 10;
        }
    }

    auto kraj = chrono::system_clock::now();
    file << "Vreme na kraj: " << tp_vo_string(kraj) << endl;
    file << endl;
    file << "Osvoeni poeni: " << poeni << endl;
    file << "Ocena: " << (poeni / num < 1 ? 5 : poeni / num) << endl;
    file << endl;
    file << "-------------------------------------------";
    file << endl;

    for (signed int i = 0; i < num; ++i) {
        file << i + 1 << ". " << odgovori[i] << (prasanja[i].odgovor == odgovori[i] ? " (✓)" : " (✗)") << endl;
    }
    file.close();

    zapishi_vo_log(_student, pocetok);

    return 0;
}
