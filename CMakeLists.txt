cmake_minimum_required(VERSION 3.10)
project(Jott_Interpretter C)

set(CMAKE_C_STANDARD 11)


add_executable(Jott_Interpretter main.c Parser/parser.c Parser/parser.h Scanner/scanner.c Scanner/scanner.h Parser/Token.h Parser/parserData.h Scanner/scannerData.h Tree_Nodes/program.c Tree_Nodes/program.h Tree_Nodes/stmt_list.c Tree_Nodes/stmt_list.h Tree_Nodes/stmt.c Tree_Nodes/stmt.h Tree_Nodes/expr.c Tree_Nodes/expr.h Tree_Nodes/asmt.c Tree_Nodes/asmt.h Tree_Nodes/d_expr.c Tree_Nodes/d_expr.h Tree_Nodes/i_expr.c Tree_Nodes/i_expr.h Tree_Nodes/s_expr.c Tree_Nodes/s_expr.h Scanner/keyword_lookup.h Tree_Nodes/f_call.c Tree_Nodes/f_call.h Tree_Nodes/p_list.c Tree_Nodes/p_list.h Tree_Nodes/type.c Tree_Nodes/type.h Global_Data/ids.c Global_Data/ids.h Semantic_Analyzer/semantic_analyzer.c Semantic_Analyzer/semantic_analyzer.h Parser/Token.c)

target_include_directories(Jott_Interpretter PRIVATE
        /usr/include/glib-2.0
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        Parser
        Scanner
        Tree_Nodes
        Global_Data
        Semantic_Analyzer
        )

target_link_libraries(Jott_Interpretter
        glib-2.0
        )