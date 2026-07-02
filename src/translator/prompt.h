#ifndef PROMPT_H
#define PROMPT_H

#include <QString>

namespace Prompt
{
    inline QString defaultPromptTemplate()
    {
        return "You are a precise OCR cleaner and professional translator. "
               "1. The user expects the source text to be in %1, but you must detect the ACTUAL language of the text. "
               "2. Fix ONLY obvious OCR artifacts and homoglyph errors. STRICT RULE: DO NOT rewrite, paraphrase, or change valid words. Preserve the exact original wording, style, and punctuation. "
               "3. Translate the text to %2. "
               "Output ONLY a valid JSON object with EXACTLY these three keys: "
               "'detected_language' (the English name of the ACTUAL detected language), "
               "'cleaned_text' (the strictly preserved source text with only OCR artifacts fixed), "
               "and 'translation'. No markdown, no extra text.\n\n"
               "Text: %3";
    }

    inline QString autoDetectPromptTemplate()
    {
        return "You are a precise OCR cleaner and professional translator. "
               "1. Detect the language of the source text. "
               "2. Fix ONLY obvious OCR artifacts and homoglyph errors. STRICT RULE: DO NOT rewrite, paraphrase, or change valid words. Preserve the exact original wording, style, and punctuation (do not change standard quotes to typographical quotes). "
               "3. Translate the text to %1. "
               "Output ONLY a valid JSON object with EXACTLY these three keys: "
               "'detected_language' (English name of the language), "
               "'cleaned_text' (the strictly preserved source text with only OCR artifacts fixed), "
               "and 'translation'. No markdown, no extra text.\n\n"
               "Text: %2";
    }
}

#endif // PROMPT_H
