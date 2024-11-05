using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.IO;
using System.Threading.Tasks;

namespace Cownloads.Tests
{
    [TestClass]
    public class CownloadsTests
    {
        // Tests for Download function
        [TestMethod]
        public async Task Download_ValidUrl()
        {
            string url = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore";
            string path = Path.Combine(Directory.GetCurrentDirectory(), "tests");

            bool result = await Cownloads.Download(url, path);

            Console.WriteLine($"Download result: {result}");
            Console.WriteLine($"File exists: {File.Exists(Path.Combine(path, "Objective-C.gitignore"))}");

            Assert.IsTrue(result);
            Assert.IsTrue(File.Exists(Path.Combine(path, "Objective-C.gitignore")));
        }

        [TestMethod]
        public async Task Download_InvalidUrl()
        {
            string url = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C++.gitignore";

            bool result = await Cownloads.Download(url);

            Assert.IsFalse(result);
        }
    }
}