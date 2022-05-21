import streamlit as st
import pandas as pd
import imageio
import numpy as np
from PIL import Image
from io import BytesIO
import matplotlib.pyplot as plt
import pickle
import os
import cv2
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)


def show_image():
    img = imageio.imread("../out/salida.ppm")
    plt.imshow(img)
    st.pyplot()

def run_filters(c, b, blur, bw):

    cmd = '../main ' + filters + ' 1 ' + ps + ' ../imgs/ashitaka.ppm ../out/salida.ppm'
    os.system(cmd)
   
# Sidebar
st.sidebar.header('Parámetros')

c = st.sidebar.slider('Contraste',-100, 100, 0, 1, '%d')
b = st.sidebar.slider('Brillo', -100, 100, 0, 1, '%d')

blur = st.sidebar.checkbox('Box blur',value=False)
bw = st.sidebar.checkbox('BlackWhite',value=False)


if st.sidebar.button('Aplicar'):
    run_filters(c, b, blur, bw)


# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')

show_image()
