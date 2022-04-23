import cv2
from skimage.metrics import structural_similarity as ssim
from skimage.metrics import mean_squared_error
from skimage.util import random_noise
import numpy as np

lightfield = cv2.imread("lightfield.png")
no_lightfield = cv2.imread("nolightfield.png")

lightfield = cv2.cvtColor(lightfield, cv2.COLOR_BGR2GRAY)
no_lightfield = cv2.cvtColor(no_lightfield, cv2.COLOR_BGR2GRAY)
noise_img = random_noise(lightfield,  mode='s&p', amount=1.0)
noise_img = np.array(255*noise_img, dtype='uint8')

rpsnr = cv2.PSNR(lightfield, no_lightfield)
rssim = ssim(lightfield, no_lightfield)
rmse = mean_squared_error(lightfield, no_lightfield)

noise_rpsnr = cv2.PSNR(lightfield, noise_img)
noise_rssim = ssim(lightfield, noise_img)
noise_rmse = mean_squared_error(lightfield, noise_img)

print(f"PSNR: {rpsnr} | SSIM: {rssim} | MSE: {rmse}")

print(f"Noise PSNR: {noise_rpsnr} | SSIM: {noise_rssim} | MSE: {noise_rmse}")
