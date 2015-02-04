# Ver >2.6
cmake_minimum_required (VERSION 2.6)

project (cbasic)

# Flags
set (CMAKE_CXX_FLAGS "-std=c++0x -ggdb")

# Version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

# Folders
set (SOURCE_DIR src)
set (BUILD_DIR build)
set (BIN_DIR ${BUILD_DIR}/bin)

# Lexer and parser targets
find_package (FLEX REQUIRED)
find_package (BISON REQUIRED)
flex_target (lexer ${SOURCE_DIR}/parser/lexer.l ${SOURCE_DIR}/parser/lexer.cc)
bison_target (parser ${SOURCE_DIR}/parser/parser.yy ${SOURCE_DIR}/parser/parser.cc)
add_flex_bison_dependency (lexer parser)

# Include dirs
include_directories (
	${SOURCE_DIR}
	)

# Config file
configure_file (
	${SOURCE_DIR}/configure.h.in
	${SOURCE_DIR}/configure.h
	)

# Header files
set (HEADER_FILES
	${SOURCE_DIR}/configure.h
	${SOURCE_DIR}/verbose.h
	${SOURCE_DIR}/error/error.h
	${SOURCE_DIR}/parser/parser-context.h
	${SOURCE_DIR}/parser/tree-walker.h
	${SOURCE_DIR}/symbols/symbol-table.h
	${SOURCE_DIR}/symbols/basic-types.h
	${SOURCE_DIR}/ilang/il-instructions.h
	${SOURCE_DIR}/ilang/il-address.h
	${SOURCE_DIR}/ilang/il-block.h

	# parser operations
	${SOURCE_DIR}/parser/operations/constant-folding.h
	${SOURCE_DIR}/parser/operations/find-symbols.h
	${SOURCE_DIR}/parser/operations/type-checking.h
	${SOURCE_DIR}/parser/operations/resolve-identifiers.h

	# parser nodes
	${SOURCE_DIR}/parser/nodes/parser-node.h
	${SOURCE_DIR}/parser/nodes/expression-node.h
	${SOURCE_DIR}/parser/nodes/value-nodes.h
	${SOURCE_DIR}/parser/nodes/operator-nodes.h
	${SOURCE_DIR}/parser/nodes/identifier-node.h
	${SOURCE_DIR}/parser/nodes/statement-nodes.h

	# flex/bison
	${BISON_parser_OUTPUT_HEADER}
	${SOURCE_DIR}/parser/lexer.h
	)

# Source files
set (SOURCE_FILES
	${SOURCE_DIR}/cbasic.cc
	${SOURCE_DIR}/error/error.cc
	${SOURCE_DIR}/parser/parser-context.cc
	${SOURCE_DIR}/parser/tree-walker.cc
	${SOURCE_DIR}/symbols/symbol-table.cc
	${SOURCE_DIR}/ilang/il-instructions.cc
	${SOURCE_DIR}/ilang/il-address.cc
	${SOURCE_DIR}/ilang/il-block.cc

	# parser operations
	${SOURCE_DIR}/parser/operations/constant-folding.cc
	${SOURCE_DIR}/parser/operations/find-symbols.cc
	${SOURCE_DIR}/parser/operations/type-checking.cc
	${SOURCE_DIR}/parser/operations/resolve-identifiers.cc
	
	# parser nodes
	${SOURCE_DIR}/parser/nodes/parser-node.cc
	${SOURCE_DIR}/parser/nodes/value-nodes.cc
	${SOURCE_DIR}/parser/nodes/operator-nodes.cc
	${SOURCE_DIR}/parser/nodes/identifier-node.cc
	${SOURCE_DIR}/parser/nodes/statement-nodes.cc

	# flex/bison
	${BISON_parser_OUTPUT_SOURCE}
	${FLEX_lexer_OUTPUTS}
	)
 
# Executables
add_executable (${BIN_DIR}/cbasic
	${SOURCE_FILES}
	${HEADER_FILES}
	)
