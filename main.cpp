
#include <array>
#include <iostream>
#include <string>

enum class fn_type {
    // A = ASSOCIATIVE, C = COMMUTATIVE
    LEFT,  // A
    RIGHT, // A
    MAX,   // AC
    MIN,   // AC
    PLUS,  // AC
    IOTA
};

enum class op_type {
    REDUCE,
    SCAN
};

enum class expr_type : bool {
    TACIT,
    EXPLICIT
};

enum class node_type {
    DINODE,  // B  combinator or BLUEBIRD (APL Model)
    TRINODE, // S' combinator or PHOENIX
    OPERATOR,
    FUNCTION,
    ARRAY
};

struct node {
    node_type type;
    node(node_type t) : type{t} {}
    virtual ~node() = default;
};

struct dinode : public node {
    std::array<node*, 2> components;
    dinode(node* a, node* b) :
        node{node_type::DINODE},
        components{{a, b}} {}
};

struct trinode : public node {
    std::array<node*, 3> components;
    trinode(node* a, node* b, node* c) :
        node{node_type::TRINODE},
        components{{a, b, c}} {}
};

struct opnode : public node {
    op_type op;
    node* fn_arg;
    opnode(op_type o, node* f) : 
        node{node_type::OPERATOR}, 
        op{o},
        fn_arg{f} {}
};

struct fnnode : public node {
    fn_type fn;
    fnnode(fn_type f) :
        node{node_type::FUNCTION},
        fn{f} {}
};

constexpr int8_t INDENT_SIZE = 3;

auto to_string(fn_type f) -> std::string {
    switch (f) {
        case fn_type::LEFT:  return "LEFT";
        case fn_type::RIGHT: return "RIGHT";
        case fn_type::MAX:   return "MAX";
        case fn_type::MIN:   return "MIN";
        case fn_type::PLUS:  return "PLUS";
        case fn_type::IOTA:  return "IOTA";
    }
    return "FAILURE";
}

void print(node* n, int indent = 0) {
    switch (n->type) {
        case node_type::DINODE:
        {
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "DINODE\n";
            auto t = dynamic_cast<dinode*>(n);
            print(t->components[0], indent + 1);
            print(t->components[1], indent + 1);
            break;
        }
        case node_type::TRINODE:
        {
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "TRINODE\n";
            auto t = dynamic_cast<trinode*>(n);
            print(t->components[0], indent + 1);
            print(t->components[1], indent + 1);
            print(t->components[2], indent + 1);
            break;
        }
        case node_type::OPERATOR:
        {
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "OPERATOR ";
            auto t = dynamic_cast<opnode*>(n);
            auto const op = t->op == op_type::SCAN ? std::string{"SCAN\n"} : std::string{"REDUCE\n"};
            std::cout << op;
            print(t->fn_arg, indent + 1);
            break;
        }
        case node_type::FUNCTION:
        {
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "OPERATOR ";
            auto t = dynamic_cast<fnnode*>(n);
            std::cout << to_string(t->fn) << '\n';
            break;
        }
    }
    if (indent == 0) std::cout << '\n';
}

auto main() -> int {

    // plus reduce iota
    node* a = new dinode{
        new opnode{op_type::REDUCE, new fnnode{fn_type::PLUS}},
        new fnnode{fn_type::IOTA}};

    print(a);

    // max reduce (right max plus) scan
    node* b = new dinode{
        new opnode{op_type::REDUCE, new fnnode{fn_type::MAX}},
        new opnode{op_type::SCAN,
            new trinode{
                new fnnode{fn_type::RIGHT},
                new fnnode{fn_type::MAX},
                new fnnode{fn_type::PLUS},
            }}};

    print(b);

    return 0;
}