#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <Cownloadpp.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

class CurlMock
{
public:
    MOCK_METHOD(void, perform, (), ());
    MOCK_METHOD(void, cleanup, (), ());
};

class CownloadppTest : public ::testing::Test
{
public:
    CownloadppTest() = default;
};

// Tests for fetchFilename function
TEST_F(CownloadppTest, FetchFilename_ValidUrl_ReturnsFilename)
{
    std::string link = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore";
    std::string expected = "Objective-C.gitignore";
    EXPECT_EQ(fetchFilename(link), expected);
}

TEST_F(CownloadppTest, FetchFilename_NoSlash_ReturnsFullUrl)
{
    std::string link = "Objective-C.gitignore";
    EXPECT_EQ(fetchFilename(link), link);
}

// Test for download function
TEST_F(CownloadppTest, Download_DirectLink_CallsDownload) {
    std::string link = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore";
    std::string path = "./tests";

    bool result = Cownloadpp::download(link, path);
    EXPECT_TRUE(result);
}

// Run the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}