import urllib3

def download(url):
    last_slash_index = url.rfind('/')
    filename = url[last_slash_index + 1:] if last_slash_index != -1 else url

    if not filename:
        raise ValueError("Invalid URL: No filename could be extracted")
        return False

    http = urllib3.PoolManager()
    response = http.request('GET', url)

    if response.status == 200:
        with open(filename, 'wb') as f:
            f.write(response.data)
        return True
    else:
        print(f"Failed to download file: {response.data}")
        return False

def download_alt(url):
    last_slash_index = url.rfind('/')
    filename = url[last_slash_index + 1:] if last_slash_index != -1 else url

    if not filename:
        raise ValueError("Invalid URL: No filename could be extracted")
        return False

    http = urllib3.PoolManager()
    response = http.request('GET', url, preload_content=False)

    if response.status == 200:
        with open(filename, 'wb') as f:
            for chunk in response.stream(1024):
                f.write(chunk)
        return True
    else:
        print(f"Failed to download file: {response.data}")
        return False
    response.release_conn()