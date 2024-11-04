import urllib.request

def download(url):
    filename = url.rfind('/')
    try:
        urllib.request.urlretrieve(url, filename)
        return True
    except Exception as e:
        print(e)
        return False
