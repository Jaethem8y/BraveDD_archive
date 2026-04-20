#include "brave_dd.h"

#include <random>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <cctype>
#include "timer.h"

using namespace BRAVE_DD;

const unsigned char ASCII = 0;
const unsigned char COMP_WORDS = 1;
const unsigned char COMP_PASSW = 2;

std::vector<uint64_t> rules(15);
std::unordered_map<uint64_t, bool> visited;

void countEdges(Forest *F ,Edge edge) {
    uint16_t lvl = edge.getNodeLevel();
    uint32_t handle = edge.getNodeHandle();
    uint64_t key = (uint64_t(lvl) << 32) | uint64_t(handle);

    if (visited.find(key) == visited.end()) {
        BRAVE_DD::Edge child0 = F->getChildEdge(lvl, handle,0);
        BRAVE_DD::Edge child1 = F->getChildEdge(lvl, handle,1);

        if (child0.getRule() == 9) {
            if (lvl - child0.getNodeLevel() > 1) {
                rules[9] ++;
            } else {
                rules[10] ++;
            }
        } else {
            rules[child0.getRule()] ++;
        }
        if (child1.getRule() == 9) {
            if (lvl - child1.getNodeLevel() > 1) {
                rules[9] ++;
            } else {
                rules[10] ++;
            }
        } else {
            rules[child1.getRule()] ++;
        }
        visited[key] = true;
        if (child0.getNodeLevel() > 0) {
            countEdges(F, child0);
        }
        if (child1.getNodeLevel() > 0) {
            countEdges(F, child1);
        }
    }
}



const uint16_t LENGTH = 24;
// const uint16_t LENGTH = 3;

class char_encoder {
        unsigned bpc;
        bool lsb;
        bool onehot;
        unsigned char alphabet;
    public:
        char_encoder(bool _lsb, bool _onehot, unsigned char alph);
        virtual ~char_encoder();

        inline unsigned bits_per_char() const { return bpc; }

        /// Return true on success, false on failure (out of range)
        bool encode_char(char c, std::vector<bool> &m) const;

    private:
        unsigned letter2value(char c) const;
        void value2bits(unsigned v, bool* bits) const;
};

char_encoder::char_encoder(bool _lsb, bool _onehot, unsigned char alph)
    : lsb(_lsb), onehot(_onehot), alphabet(alph)
{
    std::cerr << "Encoding: " << (lsb ? "LSB, " : "MSB, ");

    if (onehot) {
        std::cerr << "one-hot, ";
        switch (alphabet) {
            // bpc = #possible excluding null
            case ASCII:         bpc = 127;  std::cerr << "ascii";
                                break;
            case COMP_WORDS:    bpc = 53;   std::cerr << "compact (words)";
                                break;
            case COMP_PASSW:    bpc = 90;   std::cerr << "compact (passwd)";
                                break;
            default:            throw "Unknown alphabet";
        }
    } else {
        std::cerr << "binary, ";
        switch (alphabet) {
            // bpc = bits required for binary encoding
            case ASCII:         bpc = 7;    std::cerr << "ascii";
                                break;
            case COMP_WORDS:    bpc = 6;    std::cerr << "compact (words)";
                                break;
            case COMP_PASSW:    bpc = 7;    std::cerr << "compact (passwd)";
                                break;
            default:            throw "Unknown alphabet";
        }
    }

    std::cerr << ". " << bpc << " bits per char\n";
}

char_encoder::~char_encoder()
{
}

bool char_encoder::encode_char(char c, std::vector<bool> &m) const
{
    static bool bits[256];
    const unsigned v = letter2value(c);
    if (0xffffffff == v) return false;
    value2bits(v, bits);
    if (lsb) {
      for (unsigned b=0; b<bpc; b++) {
          m.push_back(bits[b]);
      }
    } else {
      for (unsigned b=bpc; b; ) {
         m.push_back(bits[--b]);
      }
    }
    return true;
}

unsigned char_encoder::letter2value(char c) const
{
    if (ASCII == alphabet) {
        unsigned x = c;
        return x & 0x7f;
    }
    if (COMP_WORDS == alphabet) {
        // ugly, but actually fast and easier to adjust later
        switch (c) {
            case 0:     return 0;
            case '-':   return 1;
            case 'A':   return 2;
            case 'B':   return 3;
            case 'C':   return 4;
            case 'D':   return 5;
            case 'E':   return 6;
            case 'F':   return 7;
            case 'G':   return 8;
            case 'H':   return 9;
            case 'I':   return 10;
            case 'J':   return 11;
            case 'K':   return 12;
            case 'L':   return 13;
            case 'M':   return 14;
            case 'N':   return 15;
            case 'O':   return 16;
            case 'P':   return 17;
            case 'Q':   return 18;
            case 'R':   return 19;
            case 'S':   return 20;
            case 'T':   return 21;
            case 'U':   return 22;
            case 'V':   return 23;
            case 'W':   return 24;
            case 'X':   return 25;
            case 'Y':   return 26;
            case 'Z':   return 27;
            case 'a':   return 28;
            case 'b':   return 29;
            case 'c':   return 30;
            case 'd':   return 31;
            case 'e':   return 32;
            case 'f':   return 33;
            case 'g':   return 34;
            case 'h':   return 35;
            case 'i':   return 36;
            case 'j':   return 37;
            case 'k':   return 38;
            case 'l':   return 39;
            case 'm':   return 40;
            case 'n':   return 41;
            case 'o':   return 42;
            case 'p':   return 43;
            case 'q':   return 44;
            case 'r':   return 45;
            case 's':   return 46;
            case 't':   return 47;
            case 'u':   return 48;
            case 'v':   return 49;
            case 'w':   return 50;
            case 'x':   return 51;
            case 'y':   return 52;
            case 'z':   return 53;

            default:    return 0xffffffff;
        }
    }

    if (COMP_PASSW == alphabet) {
        // ugly, but actually fast and easier to adjust later
        switch (c) {
            case 0:     return 0;
            case '-':   return 1;
            case 'A':   return 2;
            case 'B':   return 3;
            case 'C':   return 4;
            case 'D':   return 5;
            case 'E':   return 6;
            case 'F':   return 7;
            case 'G':   return 8;
            case 'H':   return 9;
            case 'I':   return 10;
            case 'J':   return 11;
            case 'K':   return 12;
            case 'L':   return 13;
            case 'M':   return 14;
            case 'N':   return 15;
            case 'O':   return 16;
            case 'P':   return 17;
            case 'Q':   return 18;
            case 'R':   return 19;
            case 'S':   return 20;
            case 'T':   return 21;
            case 'U':   return 22;
            case 'V':   return 23;
            case 'W':   return 24;
            case 'X':   return 25;
            case 'Y':   return 26;
            case 'Z':   return 27;
            case 'a':   return 28;
            case 'b':   return 29;
            case 'c':   return 30;
            case 'd':   return 31;
            case 'e':   return 32;
            case 'f':   return 33;
            case 'g':   return 34;
            case 'h':   return 35;
            case 'i':   return 36;
            case 'j':   return 37;
            case 'k':   return 38;
            case 'l':   return 39;
            case 'm':   return 40;
            case 'n':   return 41;
            case 'o':   return 42;
            case 'p':   return 43;
            case 'q':   return 44;
            case 'r':   return 45;
            case 's':   return 46;
            case 't':   return 47;
            case 'u':   return 48;
            case 'v':   return 49;
            case 'w':   return 50;
            case 'x':   return 51;
            case 'y':   return 52;
            case 'z':   return 53;
            case '0':   return 54;
            case '1':   return 55;
            case '2':   return 56;
            case '3':   return 57;
            case '4':   return 58;
            case '5':   return 59;
            case '6':   return 60;
            case '7':   return 61;
            case '8':   return 62;
            case '9':   return 63;
            case '!':   return 64;
            case '@':   return 65;
            case '#':   return 66;
            case '$':   return 67;
            case '%':   return 68;
            case '^':   return 69;
            case '&':   return 70;
            case '*':   return 71;
            case '(':   return 72;
            case ')':   return 73;
            case '_':   return 74;
            case '+':   return 75;
            case '=':   return 76;
            case '{':   return 77;
            case '}':   return 78;
            case '[':   return 79;
            case ']':   return 80;
            case ':':   return 81;
            case ';':   return 82;
            case '\'':  return 83;
            case '"':   return 84;
            case '<':   return 85;
            case ',':   return 86;
            case '>':   return 87;
            case '.':   return 88;
            case '?':   return 89;
            case '/':   return 90;
                        // what about ~ and `

            default:    return 0xffffffff;
        }
    }

    return 0xffffffff;
}


void char_encoder::value2bits(unsigned v, bool* bits) const
{
    if (onehot) {
        for (unsigned i=0; i<bpc; i++) bits[i] = false;
        if (0==v) return;
        --v;
        bits[v] = true;
    } else {
        for (unsigned i=0; i<bpc; i++) {
            bits[i] = (v & 0x01);
            v >>= 1;
        }
    }
}



/*
 * main program stuff
 */

// int test_encoding(const char_encoder &enc)
// {

//     for (unsigned char c=0; c<128; c++) {
//         if (!enc.encode_char(c, m)) continue;

//         if (c<32) {
//             std::cout << std::hex << std::setw(2) << unsigned(c) << std::dec << ' ';
//         } else {
//             std::cout << "'" << c << "'";
//         }
//         std::cout << " -> " << m.get() << "\n";
//     }

//     return 0;
// }

int usage(const char* exe)
{
    const char* base = exe;
    for (; *exe; ++exe) {
        if ('/' == *exe) base = exe+1;
    }
    std::cerr << "Usage: " << base << " (switches)\n\n";
    std::cerr << "Reads words from a text file (one word per line) on standard input, and\n";
    std::cerr << "builds a BDD, which is written to standard output.\n\n";
    std::cerr << "Switches:\n";
    std::cerr << "    -h: This help\n";
    std::cerr << "    -t: Test character encoding and quit\n";
    std::cerr << "\n";
    std::cerr << "    -d: Display the BDD forest when done\n";
    std::cerr << "\n";
    std::cerr << "    -c: Use complement bits in BDD\n";
    std::cerr << "    -f: Build a fully-reduced BDD\n";
    std::cerr << "    -q: Build a quasi-reduced BDD\n";
    std::cerr << "\n";
    std::cerr << "    -A: ASCII (7 bit) alphabet (default)\n";
    std::cerr << "    -C: Compact alphabet (upper, lower, hyphen, null)\n";
    std::cerr << "    -P: Password alphabet (letters, digits, symbols, null)\n";
    std::cerr << "\n";
    std::cerr << "    -1: One-hot encoding\n";
    std::cerr << "    -2: Binary encoding\n";
    std::cerr << "\n";
    std::cerr << "    -L: LSB of character is top-most level\n";
    std::cerr << "    -M: MSB of character is top-most level\n";
    std::cerr << "\n";
    std::cerr << "    -I: Invert the output\n";
    std::cerr << "\n";
    std::cerr << "    -w len:  Set the max word length (default 24)\n";
    std::cerr << "             Legal range is 1 through 100.\n";
    std::cerr << "\n";
    std::cerr << "Written by Andrew Miner, 11/17/2022\n\n";
    return 1;
}

bool read_line(std::istream &in, char* word, unsigned wlen)
{
    unsigned i;
    int c;
    for (i=0; i<wlen; i++) {
        c = in.get();
        if (EOF == c) return false;
        if ('\n' == c) {
            for (; i<wlen; i++) {
                word[i] = 0;
            }
            return true;
        }
        word[i] = c;
    }
    if ('\n' == in.peek()) return true;

    // truncated word
    for (;;) {
        c = in.get();
        if (EOF == c) return false;
        if ('\n' == c) {
            return true;
        }
    }
}

int main(int argc, char** argv)
{
    unsigned wordlen = LENGTH;
    bool display = false;
    bool fully = true;
    bool complemented = false;
    unsigned char alphabet = ASCII;
    // unsigned char alphabet = COMP_WORDS;
    // bool onehot = true;
    bool onehot = false;
    bool lsbtop = false;
    bool inverted = false;

    bool test_only = false;

    std::ifstream file("/Users/jaehyeokchoi/Desktop/words.txt");
    if (!file) {
        std::cout << "File is not found" << std::endl;
        exit(0);
    }

    //
    // Set up word -> binary encoding
    //
    char_encoder enc(lsbtop, onehot, alphabet);

    // if (test_only) return test_encoding(enc);

    std::vector<bool> m;

    //
    // Read input file, one word per line
    //

    char* word = new char[wordlen+1];
    word[wordlen] = 0;

    unsigned skipped = 0;
    char* first_skip = 0;
    unsigned  mt = 0;
#ifdef SHOW_COUNT
    std::cerr << "index: " << std::setw(9) << mt << " words: " << std::setw(9) << word << " nodes: " << std::setw(9) << BDD.curr_nodes();
#endif
    // ForestSetting setting(PredefForest::REXBDD, LENGTH*53);
    // ForestSetting setting(PredefForest::REXBDD, LENGTH*127);
    // ForestSetting setting(PredefForest::REXBDD, LENGTH*6);
    // ForestSetting setting(PredefForest::REXBDD, LENGTH*7);
    //
    // ForestSetting setting(PredefForest::ROAR, LENGTH*53);
    // ForestSetting setting(PredefForest::ROAR, LENGTH*127);
    // ForestSetting setting(PredefForest::ROAR, LENGTH*6);
    ForestSetting setting(PredefForest::ROAR, LENGTH*7);


    Forest* forest = new Forest(setting);
    forest->getSetting().output(std::cout);
    Func res(forest);
    ExplictFunc ex;
    while (read_line(file, word, wordlen)) {
        m = {};
        
        bool ok = true;
        for (unsigned i=0; i<wordlen; i++) {
            ok = enc.encode_char(word[i], m);
            if (!ok) break;
        }
        if (!ok) {
            ++skipped;
            if (!first_skip) {
                first_skip = strdup(word);
            }
            continue;
        }
        std::reverse(m.begin(), m.end());
        ex.addAssignment(m, Value(1));
        ++mt;
    }

    std::cout << "::::::::::" << std::endl;
    std::cout << "mt: " << mt << std::endl;
    std::cout << "word: " << word << std::endl;
    res = ex.buildFunc(forest);
    std::cout << "node: " << forest->getNodeManUsed(res) << std::endl;
    // Edge root = res.getEdge();
    // rules[root.getRule()] ++;
    // countEdges(forest, root);
    return 0;
}
