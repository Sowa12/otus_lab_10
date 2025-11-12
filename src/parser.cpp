#include <parser.h>

CommandParser::CommandParser(const size_t size)
    : ICommandParser(size), _container(), _countOpen(0), id(CommandParser::getId())
{
}

void CommandParser::parse(const std::string &command)
{
    switch (_parseState)
    {
    case ParserState::Empty:
    {
        if (command != DYNAMIC_END)
        {
            if (command != DYNAMIC_START)
            {
                _countOpen = 0;
                _parseState = ParserState::ParseStaticBlock;
                push(command);
                if (_container.commands.size() >= _staticSize)
                {
                    _parseState = ParserState::EndParse;
                    parse(command);
                }
            }
            else
            {
                _parseState = ParserState::ParseDynamicBlock;
                _countOpen = 1;
            }
        }
    }
    break;
    case ParserState::ParseStaticBlock:
    {
        if (command == DYNAMIC_START)
        {
            _parseState = ParserState::EndParseAndDynamicBlock;
            ++_countOpen;
        }
        else if (command != DYNAMIC_END)
        {
            push(command);
            if (_container.commands.size() >= _staticSize)
            {
                _parseState = ParserState::EndParse;
                parse(command);
            }
        }
    }
    break;
    case ParserState::ParseDynamicBlock:
    {
        if (command != DYNAMIC_START)
        {
            if (command == DYNAMIC_END)
            {
                --_countOpen;
                if (_countOpen == 0)
                {
                    _parseState = ParserState::EndParse;
                    parse(command);
                }
            }
            else
            {
                push(command);
            }
        }
        else
        {
            ++_countOpen;
        }
    }
    break;
    case ParserState::EndParse:
    {
        _lastContainer = _container;
        tryClear();
        _parseState = ParserState::Empty;
    }
    break;
    case ParserState::EndParseAndDynamicBlock:
    {
        _lastContainer = _container;
        tryClear();
        _parseState = ParserState::ParseDynamicBlock;
        parse(command);
    }
    break;
    }
}

bool CommandParser::isBlockEnd()
{
    // return _parseState == ParserState::EndParse || _parseState == ParserState::EndParseAndDynamicBlock;
    bool flg = !_lastContainer.commands.empty();
    return flg;
}

CommandsWithTime CommandParser::getCommands()
{
    return _container;
}

CommandsWithTime CommandParser::getLastCommands()
{
    auto cont =  _lastContainer;
    _lastContainer = CommandsWithTime();
    return cont;
}

void CommandParser::push(const std::string &command)
{
    if (!_container.commands.size()) {
        _container.id = id;
        _container.startTime = std::time(nullptr);
    }
    _container.commands.push_back(command);
}

void CommandParser::tryClear()
{
    if (!_container.commands.empty())
    {
        _container.startTime = 0;
        _container.commands.clear();
        _parseState = ParserState::Empty;
    }
}