import unittest
from unittest.mock import patch, MagicMock
import pywload

class TestDownload(unittest.TestCase):
       
    @patch('urllib3.PoolManager')
    def test_download_success(self, mock_pool_manager):
        mock_response = MagicMock()
        mock_response.status = 200
        mock_response.data = b'asdasd'
        mock_pool_manager.return_value.request.return_value = mock_response

        url = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore"
        result = pywload.download(url)

        self.assertTrue(result)
        mock_pool_manager.return_value.request.assert_called_once_with('GET', url)

    @patch('urllib3.PoolManager')
    def test_download_failure(self, mock_pool_manager):
        mock_response = MagicMock()
        mock_response.status = 404
        mock_pool_manager.return_value.request.return_value = mock_response

        url = "https://raw.githubusercontent.com/github/gitignore/refs/heads/main/Objective-C.gitignore"
        result = pywload.download(url)

        self.assertFalse(result)
        mock_pool_manager.return_value.request.assert_called_once_with('GET', url)

if __name__ == "__main__":
    unittest.main()