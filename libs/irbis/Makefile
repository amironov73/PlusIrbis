#
# Network client library
#

AR 	     := ar
CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -O2 -Wno-unknown-pragmas -I include -I ../../include -I ../../third

SRCDIR  := src
OBJDIR  := obj
BINDIR  := bin

TARGETS := $(BINDIR)/libirbis.a

SOURCES := src/Address.cpp src/AlphabetTable.cpp src/Author.cpp src/BookInfo.cpp src/ByteNavigator.cpp src/ChunkedBuffer.cpp src/ClientQuery.cpp src/ClientSocket.cpp src/Codes.cpp src/Connection.cpp src/ConnectionAdmin.cpp src/ConnectionBase.cpp src/ConnectionContext.cpp src/ConnectionFactory.cpp src/ConnectionFull.cpp src/ConnectionLite.cpp src/ConnectionPhantom.cpp src/ConnectionSearch.cpp src/DatabaseInfo.cpp src/Date.cpp src/DirectAccess.cpp src/Directory.cpp src/Ean.cpp src/EmbeddedField.cpp src/Encoding.cp1251.cpp src/Encoding.cp866.cpp src/Encoding.cpp src/Encoding.koi8r.cpp src/Encoding.utf8.cpp src/Exemplar.cpp src/File.cpp src/FileSpecification.cpp src/FoundLine.cpp src/Gbl.cpp src/IlfFile.cpp src/IniFile.cpp src/IO.cpp src/irbis.cpp src/Isbn.cpp src/Iso2709.cpp src/Lite.cpp src/Log.cpp src/MarcRecord.cpp src/MemoryPool.cpp src/Menu.cpp src/Mst.cpp src/NewEncoding.cpp src/NumberText.cpp src/OptFile.cpp src/ParFile.cpp src/Pft.cpp src/Phantom.cpp src/ProcessInfo.cpp src/RawRecord.cpp src/Reader.cpp src/RecordField.cpp src/RecordSerializer.cpp src/RecordStatus.cpp src/Registration.cpp src/Search.cpp src/ServerResponse.cpp src/ServerStat.cpp src/Span.cpp src/SubField.cpp src/Tcp4Socket.cpp src/TermInfo.cpp src/TermPosting.cpp src/Text.cpp src/TextNavigator.cpp src/Title.cpp src/TreeFile.cpp src/TreeNode.cpp src/Upc.cpp src/UserInfo.cpp src/Version.cpp src/Visit.cpp src/Xrf.cpp
OBJ     := obj/Address.o obj/AlphabetTable.o obj/Author.o obj/BookInfo.o obj/ByteNavigator.o obj/ChunkedBuffer.o obj/ClientQuery.o obj/ClientSocket.o obj/Codes.o obj/Connection.o obj/ConnectionAdmin.o obj/ConnectionBase.o obj/ConnectionContext.o obj/ConnectionFactory.o obj/ConnectionFull.o obj/ConnectionLite.o obj/ConnectionPhantom.o obj/ConnectionSearch.o obj/DatabaseInfo.o obj/Date.o obj/DirectAccess.o obj/Directory.o obj/Ean.o obj/EmbeddedField.o obj/Encoding.cp1251.o obj/Encoding.cp866.o obj/Encoding.o obj/Encoding.koi8r.o obj/Encoding.utf8.o obj/Exemplar.o obj/File.o obj/FileSpecification.o obj/FoundLine.o obj/Gbl.o obj/IlfFile.o obj/IniFile.o obj/IO.o obj/irbis.o obj/Isbn.o obj/Iso2709.o obj/Lite.o obj/Log.o obj/MarcRecord.o obj/MemoryPool.o obj/Menu.o obj/Mst.o obj/NewEncoding.o obj/NumberText.o obj/OptFile.o obj/ParFile.o obj/Pft.o obj/Phantom.o obj/ProcessInfo.o obj/RawRecord.o obj/Reader.o obj/RecordField.o obj/RecordSerializer.o obj/RecordStatus.o obj/Registration.o obj/Search.o obj/ServerResponse.o obj/ServerStat.o obj/Span.o obj/SubField.o obj/Tcp4Socket.o obj/TermInfo.o obj/TermPosting.o obj/Text.o obj/TextNavigator.o obj/Title.o obj/TreeFile.o obj/TreeNode.o obj/Upc.o obj/UserInfo.o obj/Version.o obj/Visit.o obj/Xrf.o

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -rf $(TARGETS) $(OBJDIR)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	-mkdir $(OBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BINDIR)/libirbis.a: $(OBJ)
	-mkdir $(BINDIR)
	$(AR) rcs $@ $(OBJ)
