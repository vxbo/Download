#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curl/curl.h>
#include <lib.hpp>
#include <fstream>

class CurlMock
{
public:
    MOCK_METHOD(CURLcode, curl_easy_perform, (CURL*), ());
    MOCK_METHOD(void, curl_easy_cleanup, (CURL*), ());
};

class CownloadTest : public ::testing::Test
{
public:

    CownloadTest() = default;
};

// Tests for fetchFilename function
TEST_F(CownloadTest, FetchFilename_ValidUrl_ReturnsFilename)
{
    std::string link = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore";
    std::string expected = "Objective-C.gitignore";
    EXPECT_EQ(fetchFilename(link), expected);
}

TEST_F(CownloadTest, FetchFilename_NoSlash_ReturnsFullUrl)
{
    std::string link = "Objective-C.gitignore";
    EXPECT_EQ(fetchFilename(link), link);
}

// Test for getFile function
TEST_F(CownloadTest, GetFile_ValidLink_WritesToFile) {
    std::string link = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore";
    std::string path = "./tests";

    bool result = Cownload::getFile(link, path);

    std::ifstream file(path + fetchFilename(link));
    EXPECT_TRUE(file.good());
    file.close();

    remove((path + fetchFilename(link)).c_str());
}

// Test for getAssets function
TEST_F(CownloadTest, GetAssets_ValidRepoAndTag_DownloadsAssets) {
    std::string repo = "NtQuery/Scylla";
    std::string tag = "v0.9.8"; 
    std::string path = "./tests";

    bool result = Cownload::getAssets(repo, tag, path);
    EXPECT_TRUE(result);

    remove((path + "Scylla_v0.9.8.rar").c_str());
}

// Test for download function
TEST_F(CownloadTest, Download_ValidGhLink_CallsGetAssets) {
    std::string link = "gh:NtQuery/Scylla#v0.9.8";
    std::string path = "./tests";

    bool result = Cownload::download(link, path);
    EXPECT_TRUE(result);
}

// Test for download function with direct link
TEST_F(CownloadTest, Download_DirectLink_CallsGetFile) {
    std::string link = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore";
    std::string path = "./tests";

    bool result = Cownload::download(link, path);
    EXPECT_TRUE(result);
}

// Run the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}