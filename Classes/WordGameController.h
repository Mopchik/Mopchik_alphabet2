#ifndef MOPCHIK_WordGame_CONTROLLER_H
#define MOPCHIK_WordGame_CONTROLLER_H

#include <cocos2d.h>
#include <codecvt>
#include <locale>

struct WordGameWord {
private:
    std::vector<std::string> wordAnswerLetters;
    std::string wordAnswer;
    int pos_x;
    int pos_y;
    bool is_horizontal;
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

public:
    int getWordAnswerLength() {
        return wordAnswerLetters.size();
    }

    std::string getWordAnswer() {
        return wordAnswer;
    }

    std::string getAnswerLetterAt(size_t index) {
        return wordAnswerLetters.at(index);
    }

    int getPosX() {
        return pos_x;
    }

    int getPosY() {
        return pos_y;
    }

    bool isHorizontal() {
        return is_horizontal;
    }

CC_CONSTRUCTOR_ACCESS:
    WordGameWord(std::string wordAnswer, int pos_x, int pos_y, bool is_horizontal) {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        this->is_horizontal = is_horizontal;
        auto notFormattedAnswer = this->convert.from_bytes(wordAnswer);
        int count = 0;
        while (count < notFormattedAnswer.length()) {
            std::string letter;
            if (notFormattedAnswer.at(count) == '!') {
                letter = this->convert.to_bytes(notFormattedAnswer.at(count + 1)) + this->convert.to_bytes(notFormattedAnswer.at(count + 2));
                count += 3;
            }
            else {
                letter = this->convert.to_bytes(notFormattedAnswer.at(count));
                count++;
            }
            this->wordAnswerLetters.push_back(letter);
            this->wordAnswer += letter;
        }
    }
};

struct WordGameOneRoundData {

private:
    std::vector<std::u16string> u16letters;
    std::vector<WordGameWord*> words;
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    int _wordGameIndex;
    int _setkaWidth;
    int _setkaHeight;

public:
    int getWordsCount() { return words.size(); }
    WordGameWord* getWordAt(size_t index) { return words.at(index); }
    int getLettersCount() { return u16letters.size(); }
    std::string getLetterAt(size_t index) { return this->convert.to_bytes(u16letters.at(index)); }
    int getRoundIndex() { return _wordGameIndex; }
    std::vector<WordGameWord*> getWords() { return words; }
    int getSetkaWidth() { return _setkaWidth; }
    int getSetkaHeight() { return _setkaHeight; }

CC_CONSTRUCTOR_ACCESS:
    WordGameOneRoundData(int wordGameIndex, 
                         std::vector<WordGameWord*> wordsAnswer,
                         std::vector<std::string> letters,
                         int setkaWidth,
                         int setkaHeight) {
        std::for_each(letters.begin(), letters.end(),
            [this](std::string letter) {
                this->u16letters.push_back(this->convert.from_bytes(letter));
            }
        );
        std::for_each(wordsAnswer.begin(), wordsAnswer.end(),
            [this](WordGameWord* wordAnswer) {
                this->words.push_back(wordAnswer);
            }
        );
        this->_wordGameIndex = wordGameIndex;
        this->_setkaWidth = setkaWidth;
        this->_setkaHeight = setkaHeight;
    }
    ~WordGameOneRoundData() {
        std::for_each(words.begin(), words.end(), [](WordGameWord* word) {
                CC_SAFE_DELETE(word);
            }
        );
        words.clear();
    }
};

class WordGameController {
private:
    WordGameOneRoundData* _roundData;
    std::vector<WordGameOneRoundData*> _allRounds;
    std::vector<bool> _areSolved;
    std::string _file;
    std::u16string _allLetters = u"ОЕАНИТФСРВЛЭЩЦУКМДЖЮШПЯЫЬГЗБЧЙХЪ";
    std::vector<std::vector<bool>> _setka;
    std::vector<std::pair<int, int>> _hintsUsed;
    int _stage;
    int _level;
    bool _stagePassed = false;
    bool _levelPassed = false;

    void writeToFile(const std::string& file);
    void readStageAndLevel();
    void readGameFileByStage(int stage);
    void initNextRound();
    void clearRounds();
    
    void setWordOnSetka(int wordIndex);
    void initSetka();

    void saveProgressToFile();
    void loadProgressFromFile();
    void removeProgressFile();
    

public:
    WordGameController(WordGameController const&) = delete;
    void operator=(WordGameController const&) = delete;
    static WordGameController* getInstance();

    std::vector<WordGameWord*> getWords() { return _roundData->getWords(); }

    std::pair<int, int> hintUsed();
    bool isOpenedOnSetka(int i, int j) { return _setka[i][j]; }

    void wordSolved(int wordIndex);
    bool allWordsSolved();
    bool isWordSolvedOnSetka(int wordIndex);
    bool isWordSolved(int wordIndex) { return _areSolved.at(wordIndex); }

    void onRoundCompleted();

    int getWordsCount() { return _roundData->getWordsCount(); }
    int getAvailableWordsCount();
    int getLettersCount() { return _roundData->getLettersCount(); }
    std::string getLetterAt(size_t index) { return _roundData->getLetterAt(index); }
    int getTempIndex() { return _roundData->getRoundIndex(); }
    int getSetkaWidth() { return _roundData->getSetkaWidth(); }
    int getSetkaHeight() { return _roundData->getSetkaHeight(); }

    int getStage() { return _stage; }
    int getLevel() { return _level; }
    
    std::u16string getOpenLetters();
    char16_t getGuessedLetter();
    int getPercent();
    
    bool stagePassed();
    bool levelPassed();

CC_CONSTRUCTOR_ACCESS:
    WordGameController();
    virtual ~WordGameController();
};

#endif
