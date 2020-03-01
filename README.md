# Metaprogramming Optimization [Temporarily Public]
Investigating the benefits and applications of compile time metaprogramming.
 
## Hi, GPU Compiler folks!
 
This work is my most recent project, and it shows my C++ competence better than any other repo of mine. Please note this project is still a work in progress!

## Constexpr SQL Expressions

This is the code base for my honors project. The end goal of the project is to create a `constexpr` SQL parser which will serialize a string literal SQL query into a type representing the relational algebra expression tree of the computation. This type can then be instantiated and evaluated at run time with tables (represented as [`sql::schema`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/sql/schema.hpp) objects). Once seeded with tables, the SQL expression object will behave like a stream where the user can query the `next` row of output or dump the entire output into an `sql::schema` object. Unfortunately only GCC 9 is supported, because of the widespread use of the C++20 feature ["Class Types in Non-Type Template Parameters"](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0732r2.pdf) (for [`cexpr::string`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/cexpr/string.hpp) objects as non-type template parameters) which is only implemented by GCC 9. The project heavily uses template recursion, template metaprogramming, variadic templates, constexpr, and recursive data types. These are all, constexpr excluded, highly analogous to **ML** based programming language features.

## `sql::schema`

`sql::schema` types support [**structured binding declarations**](https://en.cppreference.com/w/cpp/language/structured_binding) which is facilitated partly through the `sql::schema` API and partly through [`std` namespace injections from `sql::row`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/sql/row.hpp#L122). The class template is parameterized on two template parameters: `Index` and `Col` variadic pack. The `Index` template argument is used to support **group by** statements by using [**SFINAE**](https://en.cppreference.com/w/cpp/language/sfinae) to select the [underlying column data container](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/sql/schema.hpp#L19) (`std::vector` or `std::multiset`). The `Index` template argument, when fully specified, provides the comparator functor used by the `std::multiset` container. Beyond this the class template provides an API for accessing the underlying container.

## `sql::row`

The row type, [`sql::row`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/sql/row.hpp) is a template recursive linked-list (similar to [`std::tuple`](https://en.cppreference.com/w/cpp/utility/tuple)). A template recursive linked-list is a template metaprogramming pattern which expresses a type analogous to a traditional linked-list. `sql::row` implements this pattern with two template parameters `Col` and `Next`. `Col` represents the [`sql::column`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/sql/column.hpp) which the node in list holds a data element from. `Next` represents the type of the next node in the list, which is either another `sql::row` type or `sql::void_row`. Because the next node is expressed as a type the template linked-list does not incur the overhead of holding a next pointer nor the run time cost of dereferencing a pointer to iterate (also makes better use of the cache). A quirk to this pattern is that the node data type need not be homogenous across the list, instead the list may be composed of heterogenous data types. Also, template linked-list access is computed at compile time, so the run time cost is constant (very nice).

## Relational Algebra Expression Nodes

Only three nodes are implemented (more to come): [`ra::projection`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/ra/projection.hpp), [`ra::rename`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/ra/rename.hpp), and [`ra::relation`](https://github.com/mkitzan/metaprogramming-optimization/blob/master/include/ra/relation.hpp). `ra::projection` and `ra::rename` are unary operators which take a single `sql::row` from their `Input` relational algebra operator and fold their operation over the row before propagating the transformed row to their `Output`. The `fold` is implemented as a template recursive operation. `ra::relation` is the only terminal in the expression tree.

## Constexpr Parsing

todo

## Template Recursive Expression Trees

todo
