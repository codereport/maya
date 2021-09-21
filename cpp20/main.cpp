
#include <array>
#include <iostream>
#include <string>
#include <vector>

enum class fn_type {
    // A = ASSOCIATIVE, C = COMMUTATIVE
    LEFT,  // A
    RIGHT, // A
    MAX,   // AC
    MIN,   // AC
    PLUS,  // AC
    TIMES, // AC
    IDENTITY,
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
    ARRAY,
    // FUSIONS
    LSR     // LIFTED_SCAN_REDUCE
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

struct array_node : public node {
    int16_t rank;
    std::vector<int> data;
    array_node(int16_t r, std::vector<int> d) :
        node{node_type::ARRAY},
        rank{r},
        data{d} {}
};

// lsr = lifted_scan_reduce
struct lsr_node : public node {
    static constexpr op_type op = op_type::REDUCE;
    node* outer_op;
    node* inner_op;
    lsr_node(node* outer, node* inner) :
        node{node_type::LSR},
        outer_op{outer},
        inner_op{inner} {}
};

void fuse(node* n) {
    switch (n->type) {
        case node_type::TRINODE:
        {
            auto t = dynamic_cast<trinode*>(n);
            for (auto& c : t->components) fuse(c);
            break;
        }
        case node_type::DINODE:
        {
            auto t = dynamic_cast<dinode*>(n);
            for (auto& c : t->components) fuse(c);

            if(t->components[0]->type == node_type::OPERATOR and 
               t->components[1]->type == node_type::OPERATOR) {
                   auto a = dynamic_cast<opnode*>(t->components[0]);
                   auto b = dynamic_cast<opnode*>(t->components[1]);
                   if (a->op == op_type::REDUCE and 
                       b->op == op_type::SCAN) {
                        delete n;
                        n = new lsr_node{a->fn_arg, b->fn_arg};
                    }
               }

            break;
        }
    }
}

constexpr int8_t INDENT_SIZE = 3;

auto to_string(fn_type f) -> std::string {
    switch (f) {
        case fn_type::LEFT:     return "LEFT";
        case fn_type::RIGHT:    return "RIGHT";
        case fn_type::MAX:      return "MAX";
        case fn_type::MIN:      return "MIN";
        case fn_type::PLUS:     return "PLUS";
        case fn_type::TIMES:    return "TIMES";
        case fn_type::IDENTITY: return "IDENTITY";
        case fn_type::IOTA:     return "IOTA";
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
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "FUNCTION ";
            auto t = dynamic_cast<fnnode*>(n);
            std::cout << to_string(t->fn) << '\n';
            break;
        }
        case node_type::ARRAY:
        {
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "ARRAY ";
            auto t = dynamic_cast<array_node*>(n);
            std::cout << "rank: " << t->rank << " data: ";
            for (auto const& e : t->data) std::cout << e << ' ';
            std::cout << '\n';
            break;
        }
        case node_type::LSR:
        {
            std::cout << std::string(indent * INDENT_SIZE, ' ') + "LSR\n";
            auto t = dynamic_cast<lsr_node*>(n);
            print(t->outer_op, indent + 1);
            print(t->inner_op, indent + 1);
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
    fuse(b);
    print(b);

    // (2 times (1 plus identity))
    node* c = new trinode{
        new array_node{0, {2}},
        new fnnode{fn_type::TIMES},
        new trinode{
            new array_node{0, {1}},
            new fnnode{fn_type::PLUS},
            new fnnode{fn_type::IDENTITY}
        }};

    print(c);

    return 0;
}