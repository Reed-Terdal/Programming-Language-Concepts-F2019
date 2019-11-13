cmake_minimum_required(VERSION 3.10)
project(jott C)

set(CMAKE_C_STANDARD 11)

add_executable(jott
        main.c
        Parser/parser.c
        Parser/parser.h
        Parser/Token.h
        Parser/Tree_Nodes/while_node.c
        Parser/Tree_Nodes/while_node.h
        Parser/Tree_Nodes/for_node.c
        Parser/Tree_Nodes/for_node.h
        Parser/Tree_Nodes/if_node.c
        Parser/Tree_Nodes/if_node.h
        Parser/Tree_Nodes/program.c
        Parser/Tree_Nodes/program.h
        Parser/Tree_Nodes/stmt_list.c
        Parser/Tree_Nodes/stmt_list.h
        Parser/Tree_Nodes/stmt.c
        Parser/Tree_Nodes/stmt.h
        Parser/Tree_Nodes/expr.c
        Parser/Tree_Nodes/expr.h
        Parser/Tree_Nodes/asmt.c
        Parser/Tree_Nodes/asmt.h
        Parser/Tree_Nodes/d_expr.c
        Parser/Tree_Nodes/d_expr.h
        Parser/Tree_Nodes/i_expr.c
        Parser/Tree_Nodes/i_expr.h
        Parser/Tree_Nodes/s_expr.c
        Parser/Tree_Nodes/s_expr.h
        Parser/Tree_Nodes/f_call.c
        Parser/Tree_Nodes/f_call.h
        Parser/Tree_Nodes/p_list.c
        Parser/Tree_Nodes/p_list.h
        Parser/Token.c
        Scanner/scanner.c Scanner/scanner.h Scanner/scannerData.h Scanner/keyword_lookup.h
        Execution/executer.c Execution/executer.h
        Global_Data/ids.c Global_Data/ids.h
        Parser/Tree_Nodes/Parse_Nodes/int_node.c
        Parser/Tree_Nodes/b_stmt_list.c
        Parser/Tree_Nodes/b_stmt_list.h
        Parser/Tree_Nodes/b_stmt.c
        Parser/Tree_Nodes/b_stmt.h
        Parser/Tree_Nodes/r_asmt.c
        Parser/Tree_Nodes/r_asmt.h
        Parser/Tree_Nodes/Parse_Nodes/int_node.h
        Parser/Tree_Nodes/Parse_Nodes/string_node.c
        Parser/Tree_Nodes/Parse_Nodes/string_node.h
        Parser/Tree_Nodes/Parse_Nodes/double_node.c
        Parser/Tree_Nodes/Parse_Nodes/double_node.h
        Parser/Tree_Nodes/Parse_Nodes/operator_node.c
        Parser/Tree_Nodes/Parse_Nodes/operator_node.h
        Parser/Tree_Nodes/Parse_Nodes/id_node.c
        Parser/Tree_Nodes/Parse_Nodes/id_node.h
        Global_Data/Errors.c Global_Data/Errors.h)

target_include_directories(jott PRIVATE
        /usr/include/glib-2.0
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        Scanner
        Parser
        Parser/Tree_Nodes
        Parser/Tree_Nodes/Parse_Nodes
        Global_Data
        Execution
        )

target_link_libraries(jott
        glib-2.0
        m
        )