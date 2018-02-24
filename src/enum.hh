#pragma once
enum request_type
{
    UNKNOWN,
    GET,
    POST
};

enum error_type
{
    NICEUH = 0,
    ACCESS_DENIED = 401,
    FORBIDDEN = 403,
    FILE_NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_ERROR = 500,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    NOT_IMPLEMENTED = 6666,
    NIQUE_TA_MERE = 6969
};