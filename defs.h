#ifndef DEFS_H
#define DEFS_H

#define ALWAYS_MANUALLY_ALGS "AlwaysManuallyAlgs"
#define LIGHT_THEME_DEFAULT "LightThemeDefault"
#define HISTORY_ENTRIES_NUM "HistoryEntriesNum"
#define PREFER_DIR_SAVING "PreferDirSaving"
#define PREFER_ALG "PreferAlg"

/* Signature - second byte of an archive */
#define SIGNATURE_HUFFMAN   0x3a
#define SIGNATURE_LZ4       0x22
#define SIGNATURE_PAQ9A     0x51

enum class HistorySortOrder{
    Latest,
    Oldest,
    NameAZ,
    NameZA
};

#endif // DEFS_H
