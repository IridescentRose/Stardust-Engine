#pragma once
#include <cstdlib>
#include <string>

/**
 * A very simple UUID utility
**/
namespace Stardust::Utilities {
	
    /**
     * UUIDs are strings
    **/
    using UUID = std::string;
    
    /**
     * Usable UUID characters
    **/
    const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    /**
     * Generic UUID generation.
    **/
    inline UUID generateUUID() {
        UUID uuid = std::string(36, ' ');
        int rnd = std::rand();
        
        uuid[8] = '-';
        uuid[13] = '-';
        uuid[18] = '-';
        uuid[23] = '-';

        uuid[14] = '4';

        for (int i = 0; i < 36; i++) {
            if (i != 8 && i != 13 && i != 18 && i != 14 && i != 23) {
                if (rnd <= 0x02) {
                    rnd = 0x2000000 + (std::rand() * 0x1000000) | 0;
                }
                rnd >>= 4;
                uuid[i] = CHARS[(i == 19) ? ((rnd & 0xf) & 0x3) | 0x8 : rnd & 0xf];
            }
        }
        return uuid;
    }
}