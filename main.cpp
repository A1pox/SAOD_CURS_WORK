#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <map>
#include <cmath>
#include <algorithm>

template<typename T>
class queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t count;

public:
    queue() : head(nullptr), tail(nullptr), count(0) {}
    
    ~queue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    queue(const queue& other) : head(nullptr), tail(nullptr), count(0) {
        Node* curr = other.head;
        while (curr) {
            push(curr->data);
            curr = curr->next;
        }
    }
    
    queue(queue&& other) noexcept : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }
    
    queue& operator=(queue&& other) noexcept {
        if (this != &other) {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            head = other.head;
            tail = other.tail;
            count = other.count;
            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }
    
    void push(const T& value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }
    
    void pop() {
        if (head) {
            Node* temp = head;
            head = head->next;
            if (!head) tail = nullptr;
            delete temp;
            count--;
        }
    }
    
    T& front() { return head->data; }
    const T& front() const { return head->data; }
    
    bool empty() const { return head == nullptr; }
    size_t size() const { return count; }
};

namespace std {
    template<typename T>
    using queue = ::queue<T>;
}

struct Record {
    char fio[32];
    char street[18];
    short house;
    short flat;
    char settleDate[10];
};

struct ListNode {
    Record data;
    ListNode* next;
    ListNode(const Record& val) : data(val), next(nullptr) {}
};

void free_list(ListNode*& head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int getch() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void clear_screen() {
    printf("\033[2J\033[1;1H");
}

std::string cp866_to_utf8(const char* src, size_t len) {
    std::string out;
    out.reserve(len * 2);

    for (size_t i = 0; i < len && src[i] != '\0'; ++i) {
        unsigned char c = static_cast<unsigned char>(src[i]);
        if (c < 128) {
            out.push_back(c);
        } else {
            static const unsigned short cp866_table[128] = {
                0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,
                0x0418,0x0419,0x041A,0x041B,0x041C,0x041D,0x041E,0x041F,
                0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,
                0x0428,0x0429,0x042A,0x042B,0x042C,0x042D,0x042E,0x042F,
                0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0436,0x0437,
                0x0438,0x0439,0x043A,0x043B,0x043C,0x043D,0x043E,0x043F,
                0x2591,0x2592,0x2593,0x2502,0x2524,0x2561,0x2562,0x2556,
                0x2555,0x2563,0x2551,0x2557,0x255D,0x255C,0x255B,0x2510,
                0x2514,0x2534,0x252C,0x251C,0x2500,0x253C,0x255E,0x255F,
                0x255A,0x2554,0x2569,0x2566,0x2560,0x2550,0x256C,0x2567,
                0x2568,0x2564,0x2565,0x2559,0x2558,0x2552,0x2553,0x256B,
                0x256A,0x2518,0x250C,0x2588,0x2584,0x258C,0x2590,0x2580,
                0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,
                0x0448,0x0449,0x044A,0x044B,0x044C,0x044D,0x044E,0x044F,
                0x0401,0x0451,0x0404,0x0454,0x0407,0x0457,0x040E,0x045E,
                0x00B0,0x2219,0x00B7,0x221A,0x2116,0x00A4,0x25A0,0x00A0
            };
            unsigned short code = cp866_table[c - 128];
            if (code < 0x80) {
                out.push_back((char)code);
            } else if (code < 0x800) {
                out.push_back(0xC0 | (code >> 6));
                out.push_back(0x80 | (code & 0x3F));
            } else {
                out.push_back(0xE0 | (code >> 12));
                out.push_back(0x80 | ((code >> 6) & 0x3F));
                out.push_back(0x80 | (code & 0x3F));
            }
        }
    }
    return out;
}

int last_non_space(const char* s, int n) {
    int i = n - 1;
    while (i >= 0 && s[i] == ' ') --i;
    return i;
}

int compareDate(const char* d1, const char* d2) {
    int y1 = (d1[6]-'0')*10 + (d1[7]-'0');
    int y2 = (d2[6]-'0')*10 + (d2[7]-'0');
    if (y1 != y2) return (y1 < y2) ? -1 : 1;

    int m1 = (d1[3]-'0')*10 + (d1[4]-'0');
    int m2 = (d2[3]-'0')*10 + (d2[4]-'0');
    if (m1 != m2) return (m1 < m2) ? -1 : 1;

    int d_1 = (d1[0]-'0')*10 + (d1[1]-'0');
    int d_2 = (d2[0]-'0')*10 + (d2[1]-'0');
    if (d_1 != d_2) return (d_1 < d_2) ? -1 : 1;

    return 0;
}

int compareStreet(const char* a, const char* b, int n = 18) {
    int len_a = n; 
    while (len_a > 0 && a[len_a - 1] == ' ') len_a--;
    
    int len_b = n; 
    while (len_b > 0 && b[len_b - 1] == ' ') len_b--;

    int min_len = (len_a < len_b) ? len_a : len_b;

    for (int i = 0; i < min_len; ++i) {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];
        if (ca < cb) return -1;
        if (ca > cb) return 1;
    }

    if (len_a < len_b) return -1;
    if (len_a > len_b) return 1;
    return 0;
}

bool recordLess(const Record& a, const Record& b) {
    int cd = compareDate(a.settleDate, b.settleDate);
    if (cd != 0) return cd < 0;
    return compareStreet(a.street, b.street) < 0;
}



std::queue<Record> merge_queues(std::queue<Record>& a, std::queue<Record>& b) {
    std::queue<Record> result;
    while (!a.empty() && !b.empty()) {
        if (recordLess(a.front(), b.front())) {
            result.push(a.front());
            a.pop();
        } else {
            result.push(b.front());
            b.pop();
        }
    }
    while (!a.empty()) {
        result.push(a.front());
        a.pop();
    }
    while (!b.empty()) {
        result.push(b.front());
        b.pop();
    }
    return result;
}

void mergeSort(ListNode*& head) {
    if (!head || !head->next) return;

    ::queue<Record> a;
    ::queue<Record> b;
    
    int k = 1;
    
    while (true) {
        ListNode* curr = head;
        int splits_count = 0;

        while (curr) {
            splits_count++;
            for (int i = 0; i < k && curr; i++) {
                a.push(curr->data);
                curr = curr->next;
            }
            for (int i = 0; i < k && curr; i++) {
                b.push(curr->data);
                curr = curr->next;
            }
        }

        free_list(head);
        head = nullptr;
        ListNode* tail = nullptr;

        if (splits_count <= 1) {
            while (!a.empty()) {
                ListNode* newNode = new ListNode(a.front());
                a.pop();
                if (!head) {
                    head = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
            while(!b.empty()) b.pop(); 
            break;
        }

        while (!a.empty() || !b.empty()) {
            int c1 = k;
            int c2 = k;
            
            while ((c1 > 0 && !a.empty()) && (c2 > 0 && !b.empty())) {
                if (recordLess(a.front(), b.front())) {
                    ListNode* newNode = new ListNode(a.front());
                    a.pop();
                    c1--;
                    if (!head) head = tail = newNode;
                    else { tail->next = newNode; tail = newNode; }
                } else {
                    ListNode* newNode = new ListNode(b.front());
                    b.pop();
                    c2--;
                    if (!head) head = tail = newNode;
                    else { tail->next = newNode; tail = newNode; }
                }
            }

            while (c1 > 0 && !a.empty()) {
                ListNode* newNode = new ListNode(a.front());
                a.pop();
                c1--;
                if (!head) head = tail = newNode;
                else { tail->next = newNode; tail = newNode; }
            }

            while (c2 > 0 && !b.empty()) {
                ListNode* newNode = new ListNode(b.front());
                b.pop();
                c2--;
                if (!head) head = tail = newNode;
                else { tail->next = newNode; tail = newNode; }
            }
        }

        k *= 2;
    }
}

void print_pages(ListNode* head) {
    if (!head) {
        printf("Список пуст.\n");
        getch();
        return;
    }

    int total = 0;
    ListNode* curr = head;
    while (curr) { total++; curr = curr->next; }

    const int page_size = 20;
    int total_pages = (total + page_size - 1) / page_size;
    int current_page = 0;

    while (true) {
        clear_screen();
        printf("Страница %d из %d\n", current_page + 1, total_pages);
        printf("+--------------------------------+--------------------+-------+-------+------------+\n");
        printf("|              ФИО               |       Улица        | Дом   | Кв.   |    Дата    |\n");
        printf("+--------------------------------+--------------------+-------+-------+------------+\n");

        curr = head;
        int skip_count = current_page * page_size;
        while (curr && skip_count > 0) {
            curr = curr->next;
            skip_count--;
        }

        for (int i = 0; i < page_size && curr; ++i) {
            const Record& r = curr->data;
            printf("| %-30s", cp866_to_utf8(r.fio, 32).c_str());
            printf("| %-18s", cp866_to_utf8(r.street, 18).c_str());
            printf("| %-5d", r.house);
            printf("| %-5d", r.flat);
            printf("| %-10s |\n", cp866_to_utf8(r.settleDate, 10).c_str());
            curr = curr->next;
        }

        printf("+--------------------------------+--------------------+-------+-------+------------+\n");
        printf("[Enter] След. стр.  [Backspace] Пред. стр.  [ESC] Выход\n");

        int key = getch();
        if (key == 27) break;
        else if (key == 10 || key == 13) {
            if (current_page < total_pages - 1) current_page++;
        }
        else if (key == 127 || key == 8) {
            if (current_page > 0) current_page--;
        }
    }
}

void print_queue_pages(::queue<Record*> q_copy) {
    if (q_copy.empty()) {
        printf("Очередь пуста.\n");
        getch();
        return;
    }
    std::vector<Record*> records;
    while (!q_copy.empty()) {
        records.push_back(q_copy.front());
        q_copy.pop();
    }
    const int page_size = 20;
    int total = records.size();
    int total_pages = (total + page_size - 1) / page_size;
    int current_page = 0;

    while (true) {
        clear_screen();
        printf("Результаты поиска: Страница %d из %d (Всего: %d)\n", current_page + 1, total_pages, total);
        printf("+--------------------------------+--------------------+-------+-------+------------+\n");
        printf("|              ФИО               |       Улица        | Дом   | Кв.   |    Дата    |\n");
        printf("+--------------------------------+--------------------+-------+-------+------------+\n");

        int start = current_page * page_size;
        int end = start + page_size;
        if (end > total) end = total;

        for (int i = start; i < end; ++i) {
            const Record* r = records[i];
            printf("| %-30s", cp866_to_utf8(r->fio, 32).c_str());
            printf("| %-18s", cp866_to_utf8(r->street, 18).c_str());
            printf("| %-5d", r->house);
            printf("| %-5d", r->flat);
            printf("| %-10s |\n", cp866_to_utf8(r->settleDate, 10).c_str());
        }

        printf("+--------------------------------+--------------------+-------+-------+------------+\n");
        printf("[Enter] След. стр.  [Backspace] Пред. стр.  [ESC] Выход\n");

        int key = getch();
        if (key == 27) break;
        else if (key == 10 || key == 13) {
            if (current_page < total_pages - 1) current_page++;
        }
        else if (key == 127 || key == 8) {
            if (current_page > 0) current_page--;
        }
    }
}

std::vector<Record*> build_index(ListNode* head) {
    std::vector<Record*> index;
    index.reserve(4000); 
    
    ListNode* curr = head;
    while (curr) {
        index.push_back(&(curr->data));
        curr = curr->next;
    }
    return index;
}

::queue<Record*> search_with_index(const std::vector<Record*>& index, int year) {
    ::queue<Record*> result;
    if (index.empty()) return result;
    int left = 0;
    int right = index.size() - 1;
    int first_occurrence = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int r_year = (index[mid]->settleDate[6] - '0') * 10 + (index[mid]->settleDate[7] - '0');
        if (r_year == year) {
            first_occurrence = mid;
            right = mid - 1;
        } else if (r_year < year) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (first_occurrence != -1) {
        for (size_t i = first_occurrence; i < index.size(); ++i) {
            int r_year = (index[i]->settleDate[6] - '0') * 10 + (index[i]->settleDate[7] - '0');
            if (r_year == year) {
                result.push(index[i]);
            } else {
                break;
            }
        }
    }
    
    return result;
}

struct AVLNode {
    short house_key;
    std::vector<Record*> residents;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Record* r) : house_key(r->house), left(nullptr), right(nullptr), height(1) {
        residents.push_back(r);
    };
};

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}
AVLNode* insert(AVLNode* node, Record* key) {
    if (!node) return new AVLNode(key);

    if (key->house < node->house_key)
        node->left = insert(node->left, key);
    else if (key->house > node->house_key)
        node->right = insert(node->right, key);
    else {
        node->residents.push_back(key);
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key->house < node->left->house_key)
        return rightRotate(node);

    if (balance < -1 && key->house > node->right->house_key)
        return leftRotate(node);

    if (balance > 1 && key->house > node->left->house_key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key->house < node->right->house_key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* search(AVLNode* root, int house) {
    if (!root) return nullptr;

    if (house == root->house_key)
        return root;

    if (house < root->house_key)
        return search(root->left, house);
    else
        return search(root->right, house);
}

void printTree(AVLNode* root, const std::string& prefix = "", bool isLeft = true) {
    if (!root) return;
    
    printf("%s", prefix.c_str());
    printf("%s", isLeft ? "├── " : "└── ");
    printf("[Дом:%d]\n", root->house_key);
    
    std::string newPrefix = prefix + (isLeft ? "│   " : "    ");
    
    if (root->left || root->right) {
        if (root->left) {
            printTree(root->left, newPrefix, true);
        } else {
            printf("");
        }
        
        if (root->right) {
            printTree(root->right, newPrefix, false);
        } else {
            printf("");
        }
    }
}

void printInOrder(AVLNode* root) {
    if (!root) return;

    printInOrder(root->left);

    for (const Record* r : root->residents) {
        printf("ФИО: %-30s", cp866_to_utf8(r->fio, 32).c_str());
        printf("| Улица: %-18s", cp866_to_utf8(r->street, 18).c_str());
        printf("| Дом: %-5d", r->house);
        printf("| Квартира: %-5d", r->flat);
        printf("| Дата: %-10s |\n", cp866_to_utf8(r->settleDate, 10).c_str());
    }
    
    printInOrder(root->right);
}

std::string fractional_to_binary(double F, int l) {
    std::string result;
    while (l--) {
        F *= 2;
        if (F >= 1.0) {
            result += '1';
            F -= 1.0;
        } else {
            result += '0';
        }
    }
    return result;
}

void fano_split(int L, int R, const std::vector<std::pair<unsigned char, double>>& probs, std::map<unsigned char, std::string>& codes, std::string current_code) {
    if (L > R) return;
    if (L == R) {
        codes[probs[L].first] = current_code;
        return;
    }

    double total_prob = 0;
    for (int i = L; i <= R; ++i) total_prob += probs[i].second;

    double current_sum = 0;
    double half_prob = total_prob / 2.0;
    int split_index = L;
    double min_diff = 1.0;

    for (int i = L; i < R; ++i) {
        current_sum += probs[i].second;
        double diff = std::abs(half_prob - current_sum);
        if (diff < min_diff) {
            min_diff = diff;
            split_index = i;
        } else {
            break;
        }
    }

    fano_split(L, split_index, probs, codes, current_code + "0");
    fano_split(split_index + 1, R, probs, codes, current_code + "1");
}

void encode_fano(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка открытия файла для кодирования.\n");
        return;
    }

    std::map<unsigned char, int> freq;
    int total = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        freq[(unsigned char)c]++;
        total++;
    }
    fclose(file);

    std::vector<std::pair<unsigned char, double>> probs;
    for (auto& [ch, count] : freq) {
        probs.emplace_back(ch, static_cast<double>(count) / total);
    }

    std::sort(probs.begin(), probs.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    std::map<unsigned char, std::string> codes;
    if (!probs.empty()) {
        if (probs.size() == 1) codes[probs[0].first] = "0";
        else fano_split(0, probs.size() - 1, probs, codes, "");
    }

    printf("\n+--------+-------------+--------+----------------------------+\n");
    printf("| Символ | Вероятность | Длина  | Код Фано                   |\n");
    printf("+--------+-------------+--------+----------------------------+\n");
    
    double avgLen = 0.0, entropy = 0.0;
    for (auto& [ch, p] : probs) {
        const std::string& code = codes[ch];
        int l = code.length();
        avgLen += p * l;
        if (p > 0) entropy += -p * std::log2(p);
        
        char temp[2] = {(char)ch, '\0'};
        std::string utf8_char = cp866_to_utf8(temp, 1);
        
        if (ch >= 32 && ch < 128)
            printf("|  %3c   | %10.6f | %7d | %-24s |\n", ch, p, l, code.c_str());
        else if (ch >= 128)
            printf("| %-5s | %10.6f | %7d | %-24s |\n", utf8_char.c_str(), p, l, code.c_str());
        else
            printf("| 0x%02X | %10.6f | %7d | %-24s |\n", ch, p, l, code.c_str());
    }
    printf("+------+------------+---------+--------------------------+\n");
    printf("Энтропия: H = %.6f бит/символ\n", entropy);
    printf("Средняя длина кода: L = %.6f бит/символ\n", avgLen);
    printf("Избыточность: R = L - H = %.6f\n", avgLen - entropy);
}

void encode_and_pack_fano(const char* input_filename, const char* output_filename) {
    FILE* file = fopen(input_filename, "rb");
    if (!file) {
        printf("Ошибка открытия файла '%s'\n", input_filename);
        return;
    }

    std::map<unsigned char, int> freq;
    int total = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        freq[(unsigned char)c]++;
        total++;
    }
    fclose(file);

    std::vector<std::pair<unsigned char, double>> probs;
    for (auto& [ch, count] : freq) {
        probs.emplace_back(ch, static_cast<double>(count) / total);
    }

    std::sort(probs.begin(), probs.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    std::map<unsigned char, std::string> codes;
    if (!probs.empty()) {
         if (probs.size() == 1) codes[probs[0].first] = "0";
         else fano_split(0, probs.size() - 1, probs, codes, "");
    }

    file = fopen(input_filename, "rb");
    FILE* out = fopen(output_filename, "wb");
    if (!file || !out) {
        printf("Ошибка работы с файлами.\n");
        return;
    }

    unsigned char buffer = 0;
    int bit_count = 0;
    long original_bits = 0;
    long compressed_bits = 0;

    while ((c = fgetc(file)) != EOF) {
        unsigned char byte = (unsigned char)c;
        const std::string& code = codes[byte];
        original_bits += 8;
        compressed_bits += code.length();

        for (char bit : code) {
            buffer <<= 1;
            if (bit == '1') buffer |= 1;
            bit_count++;

            if (bit_count == 8) {
                fwrite(&buffer, 1, 1, out);
                bit_count = 0;
                buffer = 0;
            }
        }
    }

    if (bit_count > 0) {
        buffer <<= (8 - bit_count);
        fwrite(&buffer, 1, 1, out);
    }

    fclose(file);
    fclose(out);

    long original_size = original_bits / 8;
    long compressed_size = (compressed_bits + 7) / 8;

    printf("\nСжатие завершено.\n");
    printf("Исходный размер: %ld байт\n", original_size);
    printf("Сжатый размер:   %ld байт\n", compressed_size);
    printf("Коэффициент сжатия: %.3fx\n", (double)original_size / compressed_size);
}

int main() {
    FILE* file = fopen("testBase4.dat", "rb");
    if (!file) {
        printf("Ошибка открытия файла testBase4.dat\n");
        return 1;
    }

    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    Record temp;
    
    while (fread(&temp, sizeof(Record), 1, file) == 1) {
        ListNode* newNode = new ListNode(temp);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);

    ::queue<Record*> search_queue_result;
    std::vector<Record*> index_array;
    bool is_sorted = false;

    while (true) {
        clear_screen();
        printf("=== МЕНЮ ===\n");
        printf("1. Просмотр списка\n");
        printf("2. Сортировка списка (Прямое слияние очередей)\n");
        printf("3. Построение индексного массива и Поиск по году\n");
        printf("4. Построить АВЛ-дерево из результатов поиска\n");
        printf("5. Кодирование (Фано)\n");
        printf("6. Упаковать файл (сжатие Фано)\n");
        printf("7. Выход\n");
        printf("\nВыберите действие (1-7): ");

        int choice = getch();

        if (choice == '1') {
            print_pages(head);
        }
        else if (choice == '2') {
            clear_screen();
            mergeSort(head);
            is_sorted = true;
            index_array.clear(); 
            print_pages(head);
        }
        else if (choice == '3') {
            if (!is_sorted) {
                clear_screen();
                printf("ОШИБКА: Выполните пункт 2\n");
                printf("Нажмите любую клавишу...");
                getch();
                continue;
            }

            if (index_array.empty()) {
                printf("Построение индексного массива...\n");
                index_array = build_index(head);
            }

            clear_screen();
            printf("Введите год (93-97): ");
            int year = 0;
            scanf("%d", &year);
            
            search_queue_result = ::queue<Record*>();
            search_queue_result = search_with_index(index_array, year);

            if (search_queue_result.empty()) {
                printf("Записей за этот год не найдено.\n");
                getch();
            } else {
                print_queue_pages(search_queue_result);
            }
        }
        else if (choice == '4') {
            clear_screen();
            if (search_queue_result.empty()) {
                printf("Ошибка: Сначала выполните поиск (пункт 3)!\n");
                getch();
                continue;
            }

            AVLNode* root = nullptr;
            ::queue<Record*> buildQ = search_queue_result;
            
            while (!buildQ.empty()) {
                root = insert(root, buildQ.front());
                buildQ.pop();
            }

            printTree(root, "", true);
            
            printf("\n-- Поиск в дереве --\nВведите номер дома: ");
            int query_house;
            scanf("%d", &query_house);
            while (getchar() != '\n');
            
            AVLNode* found = search(root, query_house);
            if (found) {
                printf("\nВ доме %d найдены жильцы:\n", found->house_key);
                for (const Record* r : found->residents) {
                    printf("  ФИО: %s | Кв: %d\n", cp866_to_utf8(r->fio, 32).c_str(), r->flat);
                }
            } else {
                printf("Дом %d не найден в выборке.\n", query_house);
            }
            getch();
        }
        else if (choice == '5') {
            clear_screen();
            printf("Кодирование файла 'testBase4.dat' методом ФАНО:\n");
            encode_fano("testBase4.dat");
            getch();
        }
        else if (choice == '6') {
            clear_screen();
            encode_and_pack_fano("testBase4.dat", "packed_base.bin");
            getch();
        }
        else if (choice == '7') {
            free_list(head);
            break;
        }
        else{
            return 0;
        }
    }

    return 0;
}

