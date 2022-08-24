#ifndef MC_CLONE_2_BLOCK_H
#define MC_CLONE_2_BLOCK_H

#include <string>

namespace block {
    struct UvCoord {
        char u;
        char v;
    };

    struct Block {
        char m_Id;
        std::string m_Name;
        UvCoord m_Top;
        UvCoord m_Side;
        UvCoord m_Bottom;
        bool m_Opaque;
    };
}

#endif
