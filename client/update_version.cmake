execute_process(
        COMMAND git describe --tags --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(TIMESTAMP BUILD_TIME "%Y-%m-%dT%H:%M:%SZ" UTC)

configure_file(
        ${IN_DIR}/version.hxx.in
        ${OUT_DIR}/version.hxx
)
