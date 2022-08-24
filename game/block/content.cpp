#include "./content.h"
namespace block {
    Block byId(char id) {
        return content[(size_t)id];
    }
}
