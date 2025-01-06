import logging
import operator
import textwrap
import numpy as np
from reedsolo import RSCodec
from collections import defaultdict
from utils.robust_solition import PRNG
from utils.droplet import Droplet
import utils.file_process as fp
from tqdm import tqdm

logging.basicConfig(level=logging.DEBUG)

# Constants
encoded_file = 'SeedData.txt'
decoded_file = 'Kanagawa.jpg'
seed_size = 4
data_size = 16
rscode_size = 5
chunk_num = 1494
count = 0
rs = RSCodec(rscode_size)
chunks = [None] * chunk_num
prng = PRNG(K=chunk_num, delta=0.05, c=0.1, np=False)
droplets = set()
done_segments = set()
chunk_to_droplets = defaultdict(set)

def identifiers_generate(droplet):
    prng.set_seed(droplet.seed)
    droplet.num_chunks = set(prng.get_src_blocks_wrap()[1])
    droplets.add(droplet)
    for chunk_num in droplet.num_chunks:
        chunk_to_droplets[chunk_num].add(droplet)

def message_pass(droplet):
    for chunk_num in (droplet.num_chunks & done_segments):
        droplet.data = np.bitwise_xor(droplet.data, chunks[chunk_num])
        droplet.num_chunks.remove(chunk_num)
        chunk_to_droplets[chunk_num].discard(droplet)
    if len(droplet.num_chunks) == 1:
        lone_chunk = droplet.num_chunks.pop()
        chunks[lone_chunk] = droplet.data
        done_segments.add(lone_chunk)
        droplets.discard(droplet)
        chunk_to_droplets[lone_chunk].discard(droplet)
        for other_droplet in chunk_to_droplets[lone_chunk].copy():
            message_pass(other_droplet)

def decoder():
    with open(encoded_file, 'r') as f:
        for count, raw_data in enumerate(f, start=1):
            # seed, data(separated by comma)
            components = raw_data.strip().split(',')
            seed = components[0]
            data = np.array(list(map(int, components[1:])), dtype=np.uint8)
            droplet = Droplet(data, int(seed), [])
            identifiers_generate(droplet)
            message_pass(droplet)
            if len(done_segments) >= chunk_num:
                break
    logging.info("After reading %d lines, we finished decoding!", count)
    outstring = b''
    logging.info("Restoring the picture now!")
    for x in tqdm(chunks):
        outstring += ''.join(map(chr, x))
    with open(decoded_file, 'wb') as f:
        f.write(outstring)
        logging.info("MD5 is %s", fp.get_md5(outstring))
    logging.info("Done!")

if __name__ == '__main__':
    decoder()