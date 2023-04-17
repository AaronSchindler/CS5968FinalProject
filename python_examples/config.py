import numpy as np

class config:
    data_path_train = '../dataset/Amazon/amazon_train.txt'
    data_path_test = '../dataset/Amazon/amazon_test.txt'
    GPUs = '0' # empty string uses only CPU
    num_threads = 44 # Only used when GPUs is empty string
    lr = 0.0001
    ###
    feature_dim = 135909
    n_classes = 670091
    n_train = 490449
    n_test = 153025
    n_epochs = 2
    batch_size = 128
    hidden_dim = 128
    ###
    log_file = 'log_amz_ss'
    ### for sampled softmax
    n_samples = n_classes//10
    ### choose the max_labels per training sample. 
    ### If the number of true labels is < max_label,
    ### we will pad the rest of them with a dummy class (see data_generator_ss in util.py)
    max_label = 1

    # Below are the numbers for the other datasets we want to use

# Amazon 670k:

# feature_dim = 135909
# num_classes = 670091
# n_train = 490449
# n_test = 153025

# Amazon 3M:

# feature_dim = 165431
# num_classes = 2812281
# num_train = 1712536
# num_test = 739665

# Amazon 3M Titles:

# feature_dim = 337067
# num_classes = 2812281
# num_train = 1717899
# num_test = 742507

# Delicious 200k:

# feature_dim = 782585
# num_classes = 205443
# num_train = 196606
# num_test = 100095

# Wiki LSHTC:

# feature_dim = 1617899
# num_classes = 325056
# num_train = 1778351
# num_test = 587084

# Wikipedia 500k:

# feature_dim = 2381304
# num_classes = 501070
# num_train = 1813391
# num_test = 783743

# Wikipedia Titles 500k:

# feature_dim = 185479
# num_classes = 501070
# num_train = 1699722
# num_test = 722678
