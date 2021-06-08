/**
 * TODO
 */

#ifndef COLOR_H
    #define COLOR_H
    #include <string>
    
    namespace Color {
        // Foreground colors
        const std::string Reset        = "\x1b[0m";
        const std::string Black        = "\x1b[30m";
        const std::string Red          = "\x1b[31m";
        const std::string Green        = "\x1b[32m";
        const std::string Yellow       = "\x1b[33m";
        const std::string Blue         = "\x1b[34m";
        const std::string Magenta      = "\x1b[35m";
        const std::string Cyan         = "\x1b[36m";
        const std::string LightGray    = "\x1b[37m";
        const std::string Gray         = "\x1b[90m";
        const std::string LightRed     = "\x1b[91m";
        const std::string LightGreen   = "\x1b[92m";
        const std::string LightYellow  = "\x1b[93m";
        const std::string LightBlue    = "\x1b[94m";
        const std::string LightMagenta = "\x1b[95m";
        const std::string LightCyan    = "\x1b[96m";
        const std::string White        = "\x1b[97m";

        // Background colors
        const std::string BGBlack        = "\x1b[40m";
        const std::string BGRed          = "\x1b[41m";
        const std::string BGGreen        = "\x1b[42m";
        const std::string BGYellow       = "\x1b[43m";
        const std::string BGBlue         = "\x1b[44m";
        const std::string BGMagenta      = "\x1b[45m";
        const std::string BGCyan         = "\x1b[46m";
        const std::string BGLightGray    = "\x1b[47m";
        const std::string BGGray         = "\x1b[100m";
        const std::string BGLightRed     = "\x1b[101m";
        const std::string BGLightGreen   = "\x1b[102m";
        const std::string BGLightYellow  = "\x1b[103m";
        const std::string BGLightBlue    = "\x1b[104m";
        const std::string BGLightMagenta = "\x1b[105m";
        const std::string BGLightCyan    = "\x1b[106m";
        const std::string BGWhite        = "\x1b[107m";
    }
#endif