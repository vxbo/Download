using System;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;

namespace Cownloads
{
    public class Cownloads
    {
        public static async Task<bool> Download(string url, string path = ".")
        {
            if (!Directory.Exists(path))
            {
                try
                {
                    Directory.CreateDirectory(path);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Directory can't be created: {path}. Error: {ex.Message}");
                    return false;
                }
            }

            string filename = FetchFilename(url);
            if (string.IsNullOrEmpty(filename))
            {
                Console.WriteLine("Filename is empty.");
                return false;
            }

            string filePath = Path.Combine(path, filename);

            try
            {
                using HttpClient client = new();
                using HttpResponseMessage response = await client.GetAsync(url);
                response.EnsureSuccessStatusCode();

                using Stream content = await response.Content.ReadAsStreamAsync(),
                    file = new FileStream(filePath, FileMode.Create, FileAccess.Write, FileShare.None);
                await content.CopyToAsync(file);
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine($"Request error: {ex.Message}");
                return false;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                return false;
            }

            return true;
        }

        public static string FetchFilename(string url)
        {
            if (string.IsNullOrEmpty(url))
            {
                return url;
            }

            int last = url.LastIndexOf('/');
            if (last == -1)
            {
                return url;
            }

            return url[(last + 1)..];
        }
    }
}