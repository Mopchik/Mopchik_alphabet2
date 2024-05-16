#include "WordGameController.h"
#include "extensions/cocos-ext.h"
#include "json/filereadstream.h"
#include "json/filewritestream.h"
#include "json/writer.h"
#include "AppDelegate.h"
#include "MyStringUtils.h"

USING_NS_CC;
using namespace rapidjson;

WordGameController* WordGameController::getInstance()
{
    static WordGameController sm_pSharedInstance;
    return &sm_pSharedInstance;
}

WordGameController::WordGameController()
{
    readStageAndLevel();
    readGameFileByStage(_stage);
    initNextRound();
    loadProgressFromFile();
}

void WordGameController::readStageAndLevel() {
    _stage = UserDefault::getInstance()->getIntegerForKey("stage", 1);
//    _stage = 1;
    _level = UserDefault::getInstance()->getIntegerForKey("level", 0);
//    _level = 0;
}

void WordGameController::readGameFileByStage(int stage) {
    clearRounds();
    _file = StringUtils::format("stage_%d.mch", stage);
    auto path = FILEUTILS->fullPathForFilename("game_levels/" + _file);

    auto fileContent = FILEUTILS->getStringFromFile(path);
    auto wordRoundsStr = MyStringUtils::column_split(fileContent, "\r");

    std::for_each(wordRoundsStr.begin(), wordRoundsStr.end(),
                  [this](std::string& string) {
                      auto lines = MyStringUtils::line_split(string, "\r");

                      auto firstLine = MyStringUtils::line_split(lines.at(0), "\t");
                      int index = std::stoi(firstLine.at(0));
                      int wordsCount = std::stoi(firstLine.at(1));
                      size_t lettersCount;
                      std::stringstream(firstLine.at(2)) >> lettersCount;

                      std::vector<std::string> letters;
                      for (size_t i = 3; i < 3 + lettersCount; i++) {
                          letters.push_back(firstLine.at(i));
                      }
                      unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
                      std::shuffle(letters.begin(), letters.end(), std::default_random_engine(seed));

                      std::vector<WordGameWord*> words;

                      auto actualWordsCount = lines.size();
                      int maxWidth = 0;
                      int maxHeight = 0;
                      for (size_t i = 1; i < actualWordsCount; ++i) {
                          auto line = MyStringUtils::line_split(lines.at(i), "\t");
                          int answerLength = std::stoi(line.at(0));
                          std::string answer = line.at(1);
                          int pos_x = std::stoi(line.at(2));
                          int pos_y = std::stoi(line.at(3));
                          bool is_horizontal = line.at(4) == "horizontal";
                          if(is_horizontal && pos_x + answerLength > maxWidth) {
                              maxWidth = pos_x + answerLength;
                          }
                          if(!is_horizontal && pos_y + answerLength > maxHeight) {
                              maxHeight = pos_y + answerLength;
                          }
                          auto word = new (std::nothrow) WordGameWord(answer, pos_x, pos_y, is_horizontal);
                          words.emplace_back(word);
                      }

                      auto round = new (std::nothrow) WordGameOneRoundData(index, words, letters, maxWidth, maxHeight);

                      if (round) this->_allRounds.emplace_back(round);
                  }
    );
}

bool WordGameController::allWordsSolved()
{
    for (int i = 0; i < _areSolved.size(); i++) {
        if (!_areSolved[i]) {
            return false;
        }
    }
    return true;
}

void WordGameController::onRoundCompleted()
{
    removeProgressFile();
    _level++;
    _levelPassed = true;
    UserDefault::getInstance()->setIntegerForKey("level", _level);
    initNextRound();
}

void WordGameController::initNextRound(){
    if (_level < _allRounds.size()) {
        _roundData = _allRounds.at(_level);
    } else {
        _stage++;
        _stagePassed = true;
        if(_stage > 25) {
            _stage = 1;
            _stagePassed = false;
        }
        _level = 0;
        UserDefault::getInstance()->setIntegerForKey("stage", _stage);
        UserDefault::getInstance()->setIntegerForKey("level", _level);
        UserDefault::getInstance()->setBoolForKey("letter_opened", true);
        readGameFileByStage(_stage);
        initNextRound();
    }
    _areSolved.clear();
    _hintsUsed.clear();
    for (int i = 0; i < _roundData->getWordsCount(); i++) {
        _areSolved.push_back(false);
    }
    
    initSetka();
}

int WordGameController::getAvailableWordsCount()
{
    int count = 0;
    for (int i = 0; i < _areSolved.size(); i++) {
        if (!_areSolved[i]) {
            count++;
        }
    }
    return count;
}

void WordGameController::saveProgressToFile()
{
    bool anyWordsSolved = false;
    for (int i = 0; i < _areSolved.size(); i++) {
        if (_areSolved[i]) {
            anyWordsSolved = true;
        }
    }
    bool anyHintsUsed = !_hintsUsed.empty();

    if ((!anyWordsSolved && !anyHintsUsed) || allWordsSolved())
        return;

    Document d;
    d.SetObject();
    Document::AllocatorType& allocator = d.GetAllocator();

    rapidjson::Value AreSolved(kArrayType);
    std::for_each(_areSolved.begin(), _areSolved.end(), [&AreSolved, &allocator](const bool tempSolved) {
        AreSolved.PushBack(rapidjson::Value().SetBool(tempSolved), allocator);
        });
    d.AddMember("are_solved", AreSolved, allocator);

    rapidjson::Value HintsUsed(kArrayType);
    std::for_each(_hintsUsed.begin(), _hintsUsed.end(), [&HintsUsed, &allocator](const std::pair<int, int> hintUsed) {
        HintsUsed.PushBack(rapidjson::Value().SetInt(hintUsed.first * 100 + hintUsed.second), allocator);
        });
    d.AddMember("hints_used", HintsUsed, allocator);

    auto fileName = StringUtils::format("mch_%02zu_%d.json", getTempIndex(), _stage);
    auto path = FileUtils::getInstance()->getWritablePath() + "/mopchik/";
    if (!FileUtils::getInstance()->isDirectoryExist(path)) FileUtils::getInstance()->createDirectory(path);
    path += fileName;
    if (FileUtils::getInstance()->isFileExist(path)) FileUtils::getInstance()->removeFile(path);

    auto fp = fopen(path.c_str(), "wb");
    std::vector<char> buffer(65536);
    FileWriteStream os(fp, buffer.data(), buffer.size());
    Writer<FileWriteStream> writer(os);
    d.Accept(writer);
    fclose(fp);
}

void WordGameController::loadProgressFromFile()
{
    auto path = FileUtils::getInstance()->getWritablePath() + "/mopchik/" + StringUtils::format("mch_%02zu_%d.json", getTempIndex(), _stage);
    if (!FileUtils::getInstance()->isFileExist(path))
        return;
    auto fp = fopen(path.c_str(), "rb");
    std::vector<char> buffer(65536);
    FileReadStream is(fp, buffer.data(), buffer.size());
    Document d;
    d.ParseStream(is);
    fclose(fp);
    
    auto areSolvedRememberedArray = d["are_solved"].GetArray();
    for (int i = 0; i < areSolvedRememberedArray.Size(); ++i) {
        _areSolved[i] = areSolvedRememberedArray[i].GetBool();
    }

    auto hintsUsedRememberedArray = d["hints_used"].GetArray();
    for (int i = 0; i < hintsUsedRememberedArray.Size(); ++i) {
        auto num = hintsUsedRememberedArray[i].GetInt();
        _hintsUsed.push_back(std::make_pair(num / 100, num % 100));
    }
    initSetka();
}

void WordGameController::removeProgressFile()
{
    auto path = FileUtils::getInstance()->getWritablePath() + "/mopchik/" + StringUtils::format("mch_%02zu_%d.json", getTempIndex(), _stage);
    if (FileUtils::getInstance()->isFileExist(path))
        FileUtils::getInstance()->removeFile(path);
}

std::u16string WordGameController::getOpenLetters() {
    return _allLetters.substr(0, 5 + _stage);
}

char16_t WordGameController::getGuessedLetter() {
    return _allLetters.at(5 + _stage);
}

bool WordGameController::stagePassed() {
    auto returned = _stagePassed;
    _stagePassed = false;
    return returned;
}

bool WordGameController::levelPassed() {
    auto returned = _levelPassed;
    _levelPassed = false;
    return returned;
}

void WordGameController::wordSolved(int wordIndex) {
    _areSolved[wordIndex] = true;
    setWordOnSetka(wordIndex);
    saveProgressToFile();
}

std::pair<int, int> WordGameController::hintUsed() {
    for(int k = 0; k < getWords().size(); ++k) {
        auto word = getWords().at(k);
        auto word_x = word->getPosX();
        auto word_y = word->getPosY();
        for(int j = 0; j < word->getWordAnswerLength(); j++) {
            if(not _setka[word_x][word_y]) {
                auto hintPoint = std::make_pair(word_x, word_y);
                _hintsUsed.push_back(hintPoint);
                _setka[word_x][word_y] = true;
                auto allWords = getWords();
                for(int k = 0; k < allWords.size(); ++k) {
                    if(!_areSolved.at(k)){
                        _areSolved[k] = isWordSolvedOnSetka(k);
                    }
                }
                saveProgressToFile();
                return hintPoint;
            }
            if(word->isHorizontal()) {
                word_x++;
            } else {
                word_y++;
            }
        }
    }
    return std::make_pair(-1, -1);
}

void WordGameController::setWordOnSetka(int wordIndex) {
    auto word = getWords().at(wordIndex);
    auto word_x = word->getPosX();
    auto word_y = word->getPosY();
    for(int j = 0; j < word->getWordAnswerLength(); j++) {
        if(!_setka[word_x][word_y]){
            _setka[word_x][word_y] = _areSolved[wordIndex];
        }
        if(word->isHorizontal()) {
            word_x++;
        } else {
            word_y++;
        }
    }
}

void WordGameController::initSetka() {
    _setka.clear();
    auto x_count = getSetkaWidth();
    auto y_count = getSetkaHeight();
    for (int i = 0; i < x_count; i++) {
        std::vector<bool> row;
        for (int j = 0; j < y_count; j++) {
            row.push_back(false);
        }
        _setka.push_back(row);
    }
    
    for(int k = 0; k < getWords().size(); ++k) {
        setWordOnSetka(k);
    }
    
    for(int k = 0; k < _hintsUsed.size(); ++k) {
        _setka[_hintsUsed[k].first][_hintsUsed[k].second] = true;
    }
}

bool WordGameController::isWordSolvedOnSetka(int wordIndex) {
    auto word = getWords().at(wordIndex);
    auto word_x = word->getPosX();
    auto word_y = word->getPosY();
    auto allGood = true;
    for(int j = 0; j < word->getWordAnswerLength(); j++) {
        if(!_setka[word_x][word_y]) {
            allGood = false;
        }
        if(word->isHorizontal()) {
            word_x++;
        } else {
            word_y++;
        }
    }
    return allGood;
}

void WordGameController::clearRounds() {
    std::for_each(_allRounds.begin(), _allRounds.end(), [](WordGameOneRoundData* data) {
                      CC_SAFE_DELETE(data);
                  }
    );
    _allRounds.clear();
}

int WordGameController::getPercent() {
    return 100 * (getTempIndex() - 1) / _allRounds.size();
}

WordGameController::~WordGameController()
{
    clearRounds();
}
