#include <iostream>
#include "include/token.h"

using namespace std;

int main() {
    Position pos(1, 2, 3);
    Token token(Token::Type::kIdentity, "123", pos);
    token.set_content("new content");
    cout << token.type_name() << endl;
    cout << token.content() << endl;
    cout << token.position() << endl;
    return 0;
}