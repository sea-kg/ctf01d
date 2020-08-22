// automaticly generated by update_resources.py

#include <checker_example_service2_checker_py.h>

REGISTRY_WSJCPP_RESOURCE_FILE(RES_checker_example_service2_checker_py)

const std::string &RES_checker_example_service2_checker_py::getFilename() {
    static const std::string s = "checker_example_service2/checker.py";
    return s;
}

const std::string &RES_checker_example_service2_checker_py::getPackAs() {
    static const std::string s = "binary";
    return s;
}

const int RES_checker_example_service2_checker_py::getBufferSize() {
    return 3408;
}

const char *RES_checker_example_service2_checker_py::getBuffer() {
    static const unsigned char b[3408] = {
        0x23, 0x21, 0x2f, 0x75, 0x73, 0x72, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x70, 0x79, 0x74, 0x68, 0x6f, 
        0x6e, 0x0a, 0x69, 0x6d, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x73, 0x79, 0x73, 0x0a, 0x69, 0x6d, 0x70, 
        0x6f, 0x72, 0x74, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x20, 0x0a, 0x69, 0x6d, 0x70, 0x6f, 0x72, 0x74, 
        0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x0a, 0x69, 0x6d, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x72, 
        0x61, 0x6e, 0x64, 0x6f, 0x6d, 0x0a, 0x69, 0x6d, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x74, 0x69, 0x6d, 
        0x65, 0x0a, 0x69, 0x6d, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x65, 0x72, 0x72, 0x6e, 0x6f, 0x0a, 0x69, 
        0x6d, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x74, 0x72, 0x61, 0x63, 0x65, 0x62, 0x61, 0x63, 0x6b, 0x0a, 
        0x0a, 0x23, 0x20, 0x70, 0x75, 0x74, 0x2d, 0x67, 0x65, 0x74, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x20, 
        0x74, 0x6f, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x20, 0x73, 0x75, 0x63, 0x63, 0x65, 
        0x73, 0x73, 0x0a, 0x64, 0x65, 0x66, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x75, 
        0x70, 0x28, 0x29, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x22, 
        0x5b, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x20, 0x69, 0x73, 0x20, 0x77, 0x6f, 0x72, 0x6b, 
        0x65, 0x64, 0x5d, 0x20, 0x2d, 0x20, 0x31, 0x30, 0x31, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x65, 0x78, 0x69, 0x74, 0x28, 0x31, 0x30, 0x31, 0x29, 0x0a, 0x0a, 0x23, 0x20, 0x73, 0x65, 0x72, 
        0x76, 0x69, 0x63, 0x65, 0x20, 0x69, 0x73, 0x20, 0x61, 0x76, 0x61, 0x69, 0x6c, 0x61, 0x62, 0x6c, 
        0x65, 0x20, 0x28, 0x61, 0x76, 0x61, 0x69, 0x6c, 0x61, 0x62, 0x6c, 0x65, 0x20, 0x74, 0x63, 0x70, 
        0x20, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x29, 0x20, 0x62, 0x75, 0x74, 0x20, 0x70, 0x72, 
        0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x20, 0x77, 0x72, 0x6f, 0x6e, 0x67, 0x20, 0x63, 0x6f, 0x75, 
        0x6c, 0x64, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x70, 0x75, 0x74, 0x2f, 0x67, 0x65, 0x74, 0x20, 0x66, 
        0x6c, 0x61, 0x67, 0x0a, 0x64, 0x65, 0x66, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 
        0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x28, 0x29, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 
        0x72, 0x69, 0x6e, 0x74, 0x28, 0x22, 0x5b, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x20, 0x69, 
        0x73, 0x20, 0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x5d, 0x20, 0x2d, 0x20, 0x31, 0x30, 0x32, 
        0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x69, 0x74, 0x28, 0x31, 0x30, 0x32, 0x29, 
        0x0a, 0x0a, 0x23, 0x20, 0x77, 0x61, 0x69, 0x74, 0x65, 0x64, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x20, 
        0x28, 0x66, 0x6f, 0x72, 0x20, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x3a, 0x20, 0x35, 0x20, 
        0x73, 0x65, 0x63, 0x29, 0x20, 0x62, 0x75, 0x74, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 
        0x20, 0x64, 0x69, 0x64, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x74, 0x69, 
        0x6d, 0x65, 0x20, 0x74, 0x6f, 0x20, 0x72, 0x65, 0x70, 0x6c, 0x79, 0x0a, 0x64, 0x65, 0x66, 0x20, 
        0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x6d, 0x75, 0x6d, 0x62, 0x6c, 0x65, 0x28, 0x29, 
        0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x22, 0x5b, 0x73, 0x65, 
        0x72, 0x76, 0x69, 0x63, 0x65, 0x20, 0x69, 0x73, 0x20, 0x6d, 0x75, 0x6d, 0x62, 0x6c, 0x65, 0x5d, 
        0x20, 0x2d, 0x20, 0x31, 0x30, 0x33, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x69, 
        0x74, 0x28, 0x31, 0x30, 0x33, 0x29, 0x0a, 0x0a, 0x23, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 
        0x65, 0x20, 0x69, 0x73, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x61, 0x76, 0x61, 0x69, 0x6c, 0x61, 0x62, 
        0x6c, 0x65, 0x20, 0x28, 0x6d, 0x61, 0x79, 0x62, 0x65, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x65, 
        0x64, 0x20, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x6f, 0x72, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 
        0x65, 0x20, 0x69, 0x73, 0x20, 0x64, 0x6f, 0x77, 0x6e, 0x29, 0x0a, 0x64, 0x65, 0x66, 0x20, 0x73, 
        0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x64, 0x6f, 0x77, 0x6e, 0x28, 0x29, 0x3a, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x22, 0x5b, 0x73, 0x65, 0x72, 0x76, 0x69, 
        0x63, 0x65, 0x20, 0x69, 0x73, 0x20, 0x64, 0x6f, 0x77, 0x6e, 0x5d, 0x20, 0x2d, 0x20, 0x31, 0x30, 
        0x34, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x69, 0x74, 0x28, 0x31, 0x30, 0x34, 
        0x29, 0x0a, 0x0a, 0x69, 0x66, 0x20, 0x6c, 0x65, 0x6e, 0x28, 0x73, 0x79, 0x73, 0x2e, 0x61, 0x72, 
        0x67, 0x76, 0x29, 0x20, 0x21, 0x3d, 0x20, 0x35, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x72, 
        0x69, 0x6e, 0x74, 0x28, 0x22, 0x5c, 0x6e, 0x55, 0x73, 0x61, 0x67, 0x65, 0x3a, 0x5c, 0x6e, 0x5c, 
        0x74, 0x22, 0x20, 0x2b, 0x20, 0x73, 0x79, 0x73, 0x2e, 0x61, 0x72, 0x67, 0x76, 0x5b, 0x30, 0x5d, 
        0x20, 0x2b, 0x20, 0x22, 0x20, 0x3c, 0x68, 0x6f, 0x73, 0x74, 0x3e, 0x20, 0x28, 0x70, 0x75, 0x74, 
        0x7c, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x29, 0x20, 0x3c, 0x66, 0x6c, 0x61, 0x67, 0x5f, 0x69, 0x64, 
        0x3e, 0x20, 0x3c, 0x66, 0x6c, 0x61, 0x67, 0x3e, 0x5c, 0x6e, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 
        0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x22, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x3a, 
        0x5c, 0x6e, 0x5c, 0x74, 0x22, 0x20, 0x2b, 0x20, 0x73, 0x79, 0x73, 0x2e, 0x61, 0x72, 0x67, 0x76, 
        0x5b, 0x30, 0x5d, 0x20, 0x2b, 0x20, 0x22, 0x20, 0x5c, 0x22, 0x31, 0x32, 0x37, 0x2e, 0x30, 0x2e, 
        0x30, 0x2e, 0x31, 0x5c, 0x22, 0x20, 0x70, 0x75, 0x74, 0x20, 0x5c, 0x22, 0x61, 0x62, 0x63, 0x64, 
        0x69, 0x66, 0x67, 0x68, 0x72, 0x5c, 0x22, 0x20, 0x5c, 0x22, 0x31, 0x32, 0x33, 0x65, 0x34, 0x35, 
        0x36, 0x37, 0x2d, 0x65, 0x38, 0x39, 0x62, 0x2d, 0x31, 0x32, 0x64, 0x33, 0x2d, 0x61, 0x34, 0x35, 
        0x36, 0x2d, 0x34, 0x32, 0x36, 0x36, 0x35, 0x35, 0x34, 0x34, 0x30, 0x30, 0x30, 0x30, 0x5c, 0x22, 
        0x20, 0x5c, 0x6e, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 
        0x22, 0x5c, 0x6e, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x69, 0x74, 0x28, 0x30, 
        0x29, 0x0a, 0x0a, 0x64, 0x65, 0x66, 0x20, 0x64, 0x65, 0x62, 0x75, 0x67, 0x28, 0x65, 0x72, 0x72, 
        0x29, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61, 0x73, 0x73, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x23, 0x20, 0x69, 0x66, 0x20, 0x69, 0x73, 0x69, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x28, 
        0x65, 0x72, 0x72, 0x2c, 0x20, 0x73, 0x74, 0x72, 0x29, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x23, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x65, 0x72, 0x72, 0x20, 0x3d, 0x20, 0x45, 0x78, 0x63, 0x65, 0x70, 
        0x74, 0x69, 0x6f, 0x6e, 0x28, 0x65, 0x72, 0x72, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 
        0x74, 0x72, 0x61, 0x63, 0x65, 0x62, 0x61, 0x63, 0x6b, 0x2e, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x5f, 
        0x65, 0x78, 0x63, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x72, 0x61, 0x69, 0x73, 
        0x65, 0x20, 0x65, 0x72, 0x72, 0x0a, 0x0a, 0x68, 0x6f, 0x73, 0x74, 0x20, 0x3d, 0x20, 0x73, 0x79, 
        0x73, 0x2e, 0x61, 0x72, 0x67, 0x76, 0x5b, 0x31, 0x5d, 0x0a, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x3d, 
        0x20, 0x34, 0x31, 0x30, 0x32, 0x0a, 0x63, 0x6f, 0x6d, 0x6d, 0x61, 0x6e, 0x64, 0x20, 0x3d, 0x20, 
        0x73, 0x79, 0x73, 0x2e, 0x61, 0x72, 0x67, 0x76, 0x5b, 0x32, 0x5d, 0x0a, 0x66, 0x5f, 0x69, 0x64, 
        0x20, 0x3d, 0x20, 0x73, 0x79, 0x73, 0x2e, 0x61, 0x72, 0x67, 0x76, 0x5b, 0x33, 0x5d, 0x0a, 0x66, 
        0x6c, 0x61, 0x67, 0x20, 0x3d, 0x20, 0x73, 0x79, 0x73, 0x2e, 0x61, 0x72, 0x67, 0x76, 0x5b, 0x34, 
        0x5d, 0x0a, 0x0a, 0x23, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x6d, 0x75, 0x6d, 
        0x62, 0x6c, 0x65, 0x20, 0x28, 0x32, 0x29, 0x20, 0x2d, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x74, 0x65, 
        0x73, 0x74, 0x20, 0x6a, 0x75, 0x72, 0x79, 0x0a, 0x23, 0x20, 0x77, 0x68, 0x69, 0x6c, 0x65, 0x20, 
        0x54, 0x72, 0x75, 0x65, 0x3a, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x2e, 0x73, 0x6c, 0x65, 0x65, 0x70, 
        0x28, 0x31, 0x30, 0x29, 0x3b, 0x0a, 0x0a, 0x64, 0x65, 0x66, 0x20, 0x70, 0x75, 0x74, 0x5f, 0x66, 
        0x6c, 0x61, 0x67, 0x28, 0x29, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x67, 0x6c, 0x6f, 0x62, 0x61, 
        0x6c, 0x20, 0x68, 0x6f, 0x73, 0x74, 0x2c, 0x20, 0x70, 0x6f, 0x72, 0x74, 0x2c, 0x20, 0x66, 0x5f, 
        0x69, 0x64, 0x2c, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x74, 
        0x72, 0x79, 0x20, 0x70, 0x75, 0x74, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x74, 0x72, 0x79, 0x3a, 0x0a, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 
        0x22, 0x74, 0x72, 0x79, 0x20, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x20, 0x22, 0x20, 0x2b, 
        0x20, 0x68, 0x6f, 0x73, 0x74, 0x20, 0x2b, 0x20, 0x22, 0x3a, 0x22, 0x20, 0x2b, 0x20, 0x73, 0x74, 
        0x72, 0x28, 0x70, 0x6f, 0x72, 0x74, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x73, 0x20, 0x3d, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x73, 0x6f, 0x63, 0x6b, 
        0x65, 0x74, 0x28, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x41, 0x46, 0x5f, 0x49, 0x4e, 0x45, 
        0x54, 0x2c, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x53, 0x4f, 0x43, 0x4b, 0x5f, 0x53, 
        0x54, 0x52, 0x45, 0x41, 0x4d, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 
        0x2e, 0x73, 0x65, 0x74, 0x74, 0x69, 0x6d, 0x65, 0x6f, 0x75, 0x74, 0x28, 0x31, 0x29, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 
        0x28, 0x28, 0x68, 0x6f, 0x73, 0x74, 0x2c, 0x20, 0x70, 0x6f, 0x72, 0x74, 0x29, 0x29, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 
        0x73, 0x2e, 0x72, 0x65, 0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 0x29, 0x0a, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x72, 0x65, 0x73, 
        0x75, 0x6c, 0x74, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x73, 
        0x65, 0x6e, 0x64, 0x28, 0x28, 0x22, 0x70, 0x75, 0x74, 0x22, 0x20, 0x2b, 0x20, 0x22, 0x5c, 0x6e, 
        0x22, 0x29, 0x2e, 0x65, 0x6e, 0x63, 0x6f, 0x64, 0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 
        0x22, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 
        0x6c, 0x74, 0x20, 0x3d, 0x20, 0x73, 0x2e, 0x72, 0x65, 0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 
        0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x73, 0x65, 0x6e, 0x64, 
        0x28, 0x28, 0x66, 0x5f, 0x69, 0x64, 0x20, 0x2b, 0x20, 0x22, 0x5c, 0x6e, 0x22, 0x29, 0x2e, 0x65, 
        0x6e, 0x63, 0x6f, 0x64, 0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x22, 0x29, 0x29, 0x0a, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 
        0x20, 0x73, 0x2e, 0x72, 0x65, 0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 0x29, 0x0a, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x73, 0x65, 0x6e, 0x64, 0x28, 0x28, 0x66, 0x6c, 
        0x61, 0x67, 0x20, 0x2b, 0x20, 0x22, 0x5c, 0x6e, 0x22, 0x29, 0x2e, 0x65, 0x6e, 0x63, 0x6f, 0x64, 
        0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x22, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x73, 0x2e, 0x72, 
        0x65, 0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x73, 0x2e, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x65, 0x78, 0x63, 0x65, 0x70, 0x74, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x74, 0x69, 
        0x6d, 0x65, 0x6f, 0x75, 0x74, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 
        0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x64, 0x6f, 0x77, 0x6e, 0x28, 0x29, 0x0a, 0x20, 0x20, 
        0x20, 0x20, 0x65, 0x78, 0x63, 0x65, 0x70, 0x74, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 
        0x65, 0x72, 0x72, 0x6f, 0x72, 0x20, 0x61, 0x73, 0x20, 0x73, 0x65, 0x72, 0x72, 0x3a, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x69, 0x66, 0x20, 0x73, 0x65, 0x72, 0x72, 0x2e, 0x65, 
        0x72, 0x72, 0x6e, 0x6f, 0x20, 0x3d, 0x3d, 0x20, 0x65, 0x72, 0x72, 0x6e, 0x6f, 0x2e, 0x45, 0x43, 
        0x4f, 0x4e, 0x4e, 0x52, 0x45, 0x46, 0x55, 0x53, 0x45, 0x44, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 
        0x64, 0x6f, 0x77, 0x6e, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x65, 
        0x6c, 0x73, 0x65, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x64, 0x65, 0x62, 0x75, 0x67, 0x28, 0x73, 0x65, 0x72, 0x72, 0x29, 0x0a, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 
        0x5f, 0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 
        0x78, 0x63, 0x65, 0x70, 0x74, 0x20, 0x45, 0x78, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 
        0x61, 0x73, 0x20, 0x65, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 
        0x62, 0x75, 0x67, 0x28, 0x65, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 
        0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x28, 0x29, 
        0x0a, 0x0a, 0x64, 0x65, 0x66, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x5f, 0x66, 0x6c, 0x61, 0x67, 
        0x28, 0x29, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x67, 0x6c, 0x6f, 0x62, 0x61, 0x6c, 0x20, 0x68, 
        0x6f, 0x73, 0x74, 0x2c, 0x20, 0x70, 0x6f, 0x72, 0x74, 0x2c, 0x20, 0x66, 0x5f, 0x69, 0x64, 0x2c, 
        0x20, 0x66, 0x6c, 0x61, 0x67, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x74, 0x72, 0x79, 0x20, 
        0x67, 0x65, 0x74, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x20, 0x3d, 0x20, 
        0x22, 0x22, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x74, 0x72, 0x79, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x22, 0x74, 0x72, 0x79, 
        0x20, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x20, 0x22, 0x20, 0x2b, 0x20, 0x68, 0x6f, 0x73, 
        0x74, 0x20, 0x2b, 0x20, 0x22, 0x3a, 0x22, 0x20, 0x2b, 0x20, 0x73, 0x74, 0x72, 0x28, 0x70, 0x6f, 
        0x72, 0x74, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x20, 0x3d, 
        0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x28, 0x73, 
        0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x41, 0x46, 0x5f, 0x49, 0x4e, 0x45, 0x54, 0x2c, 0x20, 0x73, 
        0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x53, 0x4f, 0x43, 0x4b, 0x5f, 0x53, 0x54, 0x52, 0x45, 0x41, 
        0x4d, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x73, 0x65, 0x74, 
        0x74, 0x69, 0x6d, 0x65, 0x6f, 0x75, 0x74, 0x28, 0x31, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x73, 0x2e, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x28, 0x28, 0x68, 0x6f, 
        0x73, 0x74, 0x2c, 0x20, 0x70, 0x6f, 0x72, 0x74, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x73, 0x2e, 0x72, 0x65, 
        0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 
        0x2e, 0x64, 0x65, 0x63, 0x6f, 0x64, 0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x22, 0x2c, 
        0x20, 0x22, 0x69, 0x67, 0x6e, 0x6f, 0x72, 0x65, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x23, 0x20, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x28, 0x72, 0x65, 0x73, 0x75, 0x6c, 
        0x74, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x73, 0x65, 0x6e, 
        0x64, 0x28, 0x28, 0x22, 0x67, 0x65, 0x74, 0x5c, 0x6e, 0x22, 0x29, 0x2e, 0x65, 0x6e, 0x63, 0x6f, 
        0x64, 0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x22, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x73, 0x2e, 
        0x72, 0x65, 0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x72, 0x65, 0x73, 0x75, 
        0x6c, 0x74, 0x2e, 0x64, 0x65, 0x63, 0x6f, 0x64, 0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 
        0x22, 0x2c, 0x20, 0x22, 0x69, 0x67, 0x6e, 0x6f, 0x72, 0x65, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x73, 0x65, 0x6e, 0x64, 0x28, 0x28, 0x66, 0x5f, 0x69, 
        0x64, 0x20, 0x2b, 0x20, 0x22, 0x5c, 0x6e, 0x22, 0x29, 0x2e, 0x65, 0x6e, 0x63, 0x6f, 0x64, 0x65, 
        0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x22, 0x29, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x73, 0x2e, 0x72, 0x65, 
        0x63, 0x76, 0x28, 0x31, 0x30, 0x32, 0x34, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x3d, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 
        0x2e, 0x64, 0x65, 0x63, 0x6f, 0x64, 0x65, 0x28, 0x22, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x22, 0x2c, 
        0x20, 0x22, 0x69, 0x67, 0x6e, 0x6f, 0x72, 0x65, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x20, 0x3d, 0x20, 0x72, 0x65, 0x73, 0x75, 0x6c, 
        0x74, 0x2e, 0x73, 0x74, 0x72, 0x69, 0x70, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x20, 0x3d, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x2e, 
        0x73, 0x70, 0x6c, 0x69, 0x74, 0x28, 0x22, 0x46, 0x4f, 0x55, 0x4e, 0x44, 0x20, 0x46, 0x4c, 0x41, 
        0x47, 0x3a, 0x20, 0x22, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x69, 0x66, 
        0x20, 0x6c, 0x65, 0x6e, 0x28, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x29, 0x20, 0x3d, 0x3d, 0x20, 0x32, 
        0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 
        0x61, 0x67, 0x32, 0x20, 0x3d, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x5b, 0x31, 0x5d, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x65, 0x6c, 0x73, 0x65, 0x3a, 0x0a, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x20, 0x3d, 
        0x20, 0x27, 0x27, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x2e, 0x63, 0x6c, 
        0x6f, 0x73, 0x65, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x63, 0x65, 0x70, 0x74, 
        0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x74, 0x69, 0x6d, 0x65, 0x6f, 0x75, 0x74, 0x3a, 
        0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 
        0x5f, 0x64, 0x6f, 0x77, 0x6e, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x63, 0x65, 
        0x70, 0x74, 0x20, 0x73, 0x6f, 0x63, 0x6b, 0x65, 0x74, 0x2e, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x20, 
        0x61, 0x73, 0x20, 0x73, 0x65, 0x72, 0x72, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x69, 0x66, 0x20, 0x73, 0x65, 0x72, 0x72, 0x2e, 0x65, 0x72, 0x72, 0x6e, 0x6f, 0x20, 0x3d, 
        0x3d, 0x20, 0x65, 0x72, 0x72, 0x6e, 0x6f, 0x2e, 0x45, 0x43, 0x4f, 0x4e, 0x4e, 0x52, 0x45, 0x46, 
        0x55, 0x53, 0x45, 0x44, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x64, 0x6f, 0x77, 0x6e, 0x28, 0x29, 
        0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x65, 0x6c, 0x73, 0x65, 0x3a, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x62, 0x75, 0x67, 
        0x28, 0x73, 0x65, 0x72, 0x72, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x63, 0x6f, 0x72, 0x72, 0x75, 
        0x70, 0x74, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x78, 0x63, 0x65, 0x70, 0x74, 0x20, 
        0x45, 0x78, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x61, 0x73, 0x20, 0x65, 0x3a, 0x0a, 
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x62, 0x75, 0x67, 0x28, 0x65, 0x29, 
        0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 
        0x5f, 0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x28, 0x29, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 
        0x69, 0x66, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x20, 0x21, 0x3d, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 
        0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x62, 0x75, 0x67, 0x28, 
        0x27, 0x66, 0x6c, 0x61, 0x67, 0x3a, 0x20, 0x5b, 0x27, 0x20, 0x2b, 0x20, 0x66, 0x6c, 0x61, 0x67, 
        0x20, 0x2b, 0x20, 0x20, 0x27, 0x5d, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x3a, 0x20, 0x5b, 0x27, 
        0x20, 0x2b, 0x20, 0x73, 0x74, 0x72, 0x28, 0x66, 0x6c, 0x61, 0x67, 0x32, 0x29, 0x20, 0x2b, 0x20, 
        0x27, 0x5d, 0x27, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 
        0x76, 0x69, 0x63, 0x65, 0x5f, 0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x28, 0x29, 0x0a, 0x0a, 
        0x0a, 0x69, 0x66, 0x20, 0x63, 0x6f, 0x6d, 0x6d, 0x61, 0x6e, 0x64, 0x20, 0x3d, 0x3d, 0x20, 0x22, 
        0x70, 0x75, 0x74, 0x22, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x75, 0x74, 0x5f, 0x66, 0x6c, 
        0x61, 0x67, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x5f, 0x66, 
        0x6c, 0x61, 0x67, 0x28, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 
        0x65, 0x5f, 0x75, 0x70, 0x28, 0x29, 0x0a, 0x0a, 0x69, 0x66, 0x20, 0x63, 0x6f, 0x6d, 0x6d, 0x61, 
        0x6e, 0x64, 0x20, 0x3d, 0x3d, 0x20, 0x22, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x22, 0x3a, 0x0a, 0x20, 
        0x20, 0x20, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x5f, 0x66, 0x6c, 0x61, 0x67, 0x28, 0x29, 0x0a, 
        0x20, 0x20, 0x20, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x75, 0x70, 0x28, 0x29
    };
    return (const char*)b;
} //::buffer() 

