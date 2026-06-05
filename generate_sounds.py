import wave
import struct
import math

def generate_tone(filename, duration, freq_start, freq_end, volume=0.5, wave_type='sine'):
    sample_rate = 44100
    num_samples = int(duration * sample_rate)
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        for i in range(num_samples):
            t = float(i) / sample_rate
            # Interpolate frequency
            freq = freq_start + (freq_end - freq_start) * (t / duration)
            
            if wave_type == 'sine':
                value = math.sin(2.0 * math.pi * freq * t)
            elif wave_type == 'square':
                value = 1.0 if math.sin(2.0 * math.pi * freq * t) > 0 else -1.0
            elif wave_type == 'noise':
                import random
                value = random.uniform(-1.0, 1.0)
                
            # Envelope (fade in/out)
            env = 1.0
            if t < 0.05:
                env = t / 0.05
            elif t > duration - 0.05:
                env = (duration - t) / 0.05
                
            sample = int(value * env * volume * 32767.0)
            # Clip
            sample = max(-32768, min(32767, sample))
            
            wav_file.writeframes(struct.pack('h', sample))

# 1. Pesca carta (swish in salita)
generate_tone('pesca.wav', 0.15, 200, 600, 0.3, 'sine')

# 2. Gioca carta (swish in discesa)
generate_tone('gioca.wav', 0.15, 600, 200, 0.3, 'sine')

# 3. Click bottone (suono breve)
generate_tone('click.wav', 0.05, 800, 800, 0.2, 'sine')

# 4. UNO (suono squillante)
generate_tone('uno.wav', 0.3, 400, 1200, 0.4, 'square')

# 5. Errore/Contestazione (basso profondo)
generate_tone('errore.wav', 0.2, 150, 100, 0.4, 'square')

# 6. Vittoria (arpeggio semplice simulato con sweep)
generate_tone('vittoria.wav', 1.0, 400, 2000, 0.4, 'sine')

print("Suoni generati con successo!")
