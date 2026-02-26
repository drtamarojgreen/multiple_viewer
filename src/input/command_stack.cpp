#include "command_stack.h"

namespace input {

void CommandStack::pushAndExecute(std::unique_ptr<ICommand> command) {
    command->execute();
    history_.push_back(std::move(command));
    redoStack_.clear();
}

void CommandStack::undo() {
    if (!history_.empty()) {
        auto cmd = std::move(history_.back());
        history_.pop_back();
        cmd->undo();
        redoStack_.push_back(std::move(cmd));
    }
}

void CommandStack::redo() {
    if (!redoStack_.empty()) {
        auto cmd = std::move(redoStack_.back());
        redoStack_.pop_back();
        cmd->execute();
        history_.push_back(std::move(cmd));
    }
}

} // namespace input
