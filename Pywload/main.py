import unittest
from unittest.mock import patch
import pywload

class testDownload(unittest.TestCase):
       
    @patch('urllib.request.urlretrieve')
    def test_download_success(self, mock_urlretrieve):
        mock_urlretrieve.return_value = None
        url = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore"

        result = pywload.download(url)

        self.assertTrue(result)
        mock_urlretrieve.assert_called_once_with(url, 66)

    @patch('urllib.request.urlretrieve')
    def test_download_failure(self, mock_urlretrieve):
        mock_urlretrieve.side_effect = Exception("Download failed")
        url = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore"

        result = pywload.download(url)

        self.assertFalse(result)  # Expecting False on failure
        mock_urlretrieve.assert_called_once_with(url, 66)  # This remains unchanged as the failure case

if __name__ == "__main__":
    unittest.main()