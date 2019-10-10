cmake_minimum_required(VERSION 3.10)
project(Jott_Interpretter C)

set(CMAKE_C_STANDARD 11)


add_executable(Jott_Interpretter
        main.c
        Parser/parser.c Parser/parser.h Parser/Token.h Parser/parserData.h  Parser/Tree_Nodes/program.c Parser/Tree_Nodes/program.h Parser/Tree_Nodes/stmt_list.c Parser/Tree_Nodes/stmt_list.h Parser/Tree_Nodes/stmt.c Parser/Tree_Nodes/stmt.h Parser/Tree_Nodes/expr.c Parser/Tree_Nodes/expr.h Parser/Tree_Nodes/asmt.c Parser/Tree_Nodes/asmt.h Parser/Tree_Nodes/d_expr.c Parser/Tree_Nodes/d_expr.h Parser/Tree_Nodes/i_expr.c Parser/Tree_Nodes/i_expr.h Parser/Tree_Nodes/s_expr.c Parser/Tree_Nodes/s_expr.h Parser/Tree_Nodes/f_call.c Parser/Tree_Nodes/f_call.h Parser/Tree_Nodes/p_list.c Parser/Tree_Nodes/p_list.h Parser/Tree_Nodes/type.c Parser/Tree_Nodes/type.h Parser/Token.c
        Scanner/scanner.c Scanner/scanner.h Scanner/scannerData.h Scanner/keyword_lookup.h
        Semantic_Analyzer/semantic_analyzer.c Semantic_Analyzer/semantic_analyzer.h
        Global_Data/ids.c Global_Data/ids.h
        Parser/Tree_Nodes/Parse_Nodes/int_node.c Parser/Tree_Nodes/Parse_Nodes/int_node.h Parser/Tree_Nodes/Parse_Nodes/string_node.c Parser/Tree_Nodes/Parse_Nodes/string_node.h Parser/Tree_Nodes/Parse_Nodes/double_node.c Parser/Tree_Nodes/Parse_Nodes/double_node.h Parser/Tree_Nodes/Parse_Nodes/operator_node.c Parser/Tree_Nodes/Parse_Nodes/operator_node.h Parser/Tree_Nodes/Parse_Nodes/id_node.c Parser/Tree_Nodes/Parse_Nodes/id_node.h)

target_include_directories(Jott_Interpretter PRIVATE
        /usr/include/glib-2.0
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        Scanner
        Parser
        Parser/Tree_Nodes
        Parser/Tree_Nodes/Parse_Nodes
        Global_Data
        Semantic_Analyzer
        )

target_link_libraries(Jott_Interpretter
        glib-2.0
        m
        )