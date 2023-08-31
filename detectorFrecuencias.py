import random

# problema encontrar los dos tonos con potencia mas alta de un dirccionario

random.seed()
signal = {500: -20, 560: -20, 900: -20, 1100: -40, 1400: -50, 355: -30}
# Frecuencias hi
FA_MIN = 900
FA_MAX = 1500
# Frecuencias lo
FB_MIN = 182
FB_MAX = 400

def get_freqs(signal):
    return sorted(signal.items(), key=lambda x: x[1], reverse=True)
    
def isFa(freq):
    return FA_MIN <= freq <= FA_MAX
    #(freq >= FA_MIN and freq <= FA_MAX)

def isFb(freq):
    return FB_MIN <= freq <= FB_MAX
    #return (freq >= FB_MIN and freq <= FB_MAX)

def get_hilo(freq):
    hi = []
    lo = []
    for tuple in freq:
        first_freq = tuple[0]
        if isFa(first_freq):
            hi.append(tuple)
        elif isFb(first_freq):
            lo.append(tuple)
    return (hi, lo)    
    #a, b = freq[0:2]
    
    #return (a, b) if a > b else (b, a)

freq = get_freqs(signal)

print(f"sorted-freq: {freq}")

hi, lo = get_hilo(freq)

print(f"hi={hi}, lo={lo}")    