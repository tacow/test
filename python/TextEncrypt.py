import sys
import base64
from Crypto.Cipher import AES

key = '10jmn!Rim^fb$^xN'
iv = 'UkUFV&m59y!OK09v'

def EncryptText(text, key, iv):
    try:
        textBytes = text.encode(encoding='utf-8')
        bytesLen = len(textBytes)
        if bytesLen % 16:
            paddingLen = 16 - bytesLen % 16
            textBytes += b'\0' * paddingLen

        aes = AES.new(key, AES.MODE_CBC, iv)
        ciphertext = aes.encrypt(textBytes)
        b64 = base64.b64encode(ciphertext)
    except Exception as e:
        print("Encrypt error: %s" % (str(e)))
        return ''
    return str(b64, encoding="utf-8")

def DecryptText(b64, key, iv):
    try:
        ciphertext = base64.b64decode(b64)
        aes = AES.new(key, AES.MODE_CBC, iv)
        textBytes = aes.decrypt(ciphertext).rstrip(b'\0')
        text = textBytes.decode(encoding='utf-8')
    except Exception as e:
        print("Decrypt error: %s" % (str(e)))
        return ''
    return text

if __name__=="__main__":
    if len(sys.argv) < 3:
        print("Usage: %s [enc/dec] [text] <key> <iv>" % sys.argv[0])
        sys.exit()
    cmd = sys.argv[1]
    text = sys.argv[2]

    if len(sys.argv) >= 5:
        key = sys.argv[3]
        iv = sys.argv[4]

    if cmd == 'enc':
        ciphertext = EncryptText(text, key, iv)
        print("%s\n" % ciphertext)
    elif cmd == 'dec':
        text = DecryptText(text, key, iv)
        print("%s\n" % text)
    else:
        print('Bad command\n')