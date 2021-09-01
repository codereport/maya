
#include <array>
#include <iostream>

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
    DINODE,  // B  combinator or BLUEBIRD
    TRINODE, // S' combinator or PHOENIX
    OPERATOR,
    FUNCTION,
    ARRAY
};

struct node {
    node_type type;
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

auto main() -> int {

    // plus reduce iota
    node* a = new dinode{
        new opnode{op_type::REDUCE, new fnnode{fn_type::PLUS}},
        new fnnode{fn_type::IOTA}};

    // max reduce (right max plus) scan
    node* b = new dinode{
        new opnode{op_type::REDUCE, new fnnode{fn_type::MAX}},
        new opnode{op_type::SCAN,
            new trinode{
                new fnnode{fn_type::RIGHT},
                new fnnode{fn_type::MAX},
                new fnnode{fn_type::PLUS},
            }}};

    return 0;
}