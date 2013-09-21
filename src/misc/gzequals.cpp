
#include "ghack.h"

//
// Like strcmp, but is lenient on .gz suffixes
//
bool gzequals (const string &a, const string &b)
{
    GH_FUNCTION()

    if (a == b) {
        return (true);
    }

    static const string gz(".gz");
    static const size_t lgz = sizeof(".gz") - 1;

    size_t la = a.size();
    size_t lb = b.size();

    if (la == lb + lgz) {
        if (strcmp(a.c_str() + lb, gz.c_str())) {
            return (false);
        }

        if (strncmp(a.c_str(), b.c_str(), lb)) {
            return (false);
        }

        return (true);

    } else if (lb == la + lgz) {

        if (strcmp(b.c_str() + la, gz.c_str())) {
            return (false);
        }

        if (strncmp(a.c_str(), b.c_str(), la)) {
            return (false);
        }

        return (true);

    } else {

        return (false);
    }
}

//
// Like strcmp, but is lenient on .gz suffixes
//
int gzstrcmp (const char* a, const char* b)
{
    GH_FUNCTION()

    if (!strcmp(a, b)) {
        return (0);
    }

    static const char * gz = ".gz";
    static const size_t lgz = sizeof(".gz") - 1;

    size_t la = strlen(a);
    size_t lb = strlen(b);

    if (la == lb + lgz) {
        if (strcmp(a + lb, gz)) {
            return (1);
        }

        if (strncmp(a, b, lb)) {
            return (1);
        }

        return (0);

    } else if (lb == la + lgz) {

        if (strcmp(b + la, gz)) {
            return (1);
        }

        if (strncmp(a, b, la)) {
            return (1);
        }

        return (0);

    } else {

        return (1);
    }
}

//
// As above, but ignores leading pathnames
//
bool gzfileequals (const string &a, const string &b)
{
    GH_FUNCTION()

    const char *ra = strrchr(a.c_str(), '/');
    const char *rb = strrchr(b.c_str(), '/');

    if (ra == NULL) {
        ra = a.c_str();
    } else {
        ra++;
    }

    if (rb == NULL) {
        rb = b.c_str();
    } else {
        rb++;
    }

    if (!gzstrcmp(ra, rb)) {
        return (true);
    }

    return (false);
}

//
// As above, but ignores leading pathnames
//
bool gzfileequals (const char* a, const char* b)
{
    GH_FUNCTION()

    const char *ra = strrchr(a, '/');
    const char *rb = strrchr(b, '/');

    if (ra == NULL) {
        ra = a;
    } else {
        ra++;
    }

    if (rb == NULL) {
        rb = b;
    } else {
        rb++;
    }

    if (!gzstrcmp(ra, rb)) {
        return (true);
    }

    return (false);
}
