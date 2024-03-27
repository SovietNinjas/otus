#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

class Document {
   public:
    void create() { std::cout << "new document" << std::endl; }

    void import(std::string &text) {
        m_text = text;
        std::cout << "import " << text << std::endl;
    }

    void exportDoc(std::string_view format) { std::cout << "export as " << format << std::endl; }

   private:
    std::string m_text{};
};

class ICommand {
   public:
    virtual void execute() = 0;  // Обычно здесь описан всего один метод для запуска команды.
    virtual ~ICommand() = default;
};

class Command : public ICommand {
   public:
    virtual ~Command() = default;

   protected:
    Command(Document *d) : document(d) {}
    Document *document;
};

class NewDocumentCommand : public Command {
   public:
    NewDocumentCommand(Document *d) : Command(d) {}

    void execute() override { document->create(); }
};

class ImportCommand : public Command {
   public:
    ImportCommand(Document *d, const std::string &text_) : Command(d), m_text(text_) {}

    void execute() override { document->import(m_text); }

   private:
    std::string m_text;
};

class ExportCommand : public Command {
   public:
    ExportCommand(Document *d, const std::string &format) : Command(d), format_(format) {}

    void execute() override { document->exportDoc(format_); }

   private:
    std::string format_;
};

class FileManager {
   public:
    inline void createNewDocument() {
        auto cmd = NewDocumentCommand(doc.get());
        commandExecute_(cmd);
    }

    inline void exportAs(const std::string& format) {
        auto cmd = ExportCommand(doc.get(), format);
        commandExecute_(cmd);
    }

    inline void import(const std::string& str) {
        auto cmd = ImportCommand(doc.get(), str);
        commandExecute_(cmd);
    }

   private:
    void commandExecute_(Command& cmd) { cmd.execute(); }
    std::unique_ptr<Document> doc;
};
