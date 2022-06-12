from os import listdir
from os.path import isfile, join
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
st.set_page_config(layout="wide")

class FiltersPage:

    def __init__(self):

        self.merge_image = None
        self.filter_values = dict()
        self.frame_hue = None
        self.filter_cmd = {
            "plain": lambda hue : f"plain {hue}",
            "blackwhite": lambda foo : f"blackwhite",
            "bright": lambda b : f"bright {b/100}",
            "shades": lambda scale : f"shades {scale}",
            "merge": lambda alpha : f"merge ../src/imgs/{self.merge_image} {alpha/100}",
            "contrast": lambda level : f"contrast {level}",
            "blur": lambda level : f"blur {level}",
            "edge": lambda level : f"edge {level}",
        }
        self.images = {} # {image: column}
        
        self.update_main_layout()
        self.update_sidebar()



    def show_images(self, images):
        if (len(images) > 0):
            columns = st.columns(len(images))
            self.images = {images[i] : columns[i] for i in range(len(images))}
        
            for image in range(len(images)):
                with columns[image]:
                    st.caption(images[image])
                    img = imageio.imread(f"../src/imgs/{images[image]}")
                    plt.imshow(img)
                    st.pyplot()
                    if os.path.exists(f"../out/{images[image].replace('.ppm', '')}_out.ppm"):
                        img_out = imageio.imread(f"../out/{images[image].replace('.ppm', '')}_out.ppm")
                        plt.imshow(img_out)
                        st.pyplot()


    def run_filters(self):
        cmd = ""
        for image in self.images:
            cmd = f"../src/imagine ../src/imgs/{image} ../out/{image.replace('.ppm', '')}_out.ppm -s"
            for filter in self.filter_values:
                if self.filter_values[filter]:
                        if self.filter_values[filter]:
                            cmd += f" -f {self.filter_cmd[filter](self.filter_values[filter])}"
                os.system(cmd)
    

    def update_sidebar(self):
        # Sidebar
        st.sidebar.title('Filters:')
        st.sidebar.header('Pixel2Pixel:')


        self.filter_values["plain"] = st.sidebar.slider('Plain',0, 255, 0, 1, '%d')
        self.filter_values["contrast"] = st.sidebar.slider('Contrast',0, 200, 0, 1, '%d')
        self.filter_values["bright"] = st.sidebar.slider('Bright',-100, 100, 0, 1, '%f')
        self.filter_values["shades"] = st.sidebar.slider('Shades',0, 255, 0, 1, '%d')
        self.filter_values["blackwhite"] = st.sidebar.checkbox('BlackWhite',value=False)
        
        mypath = "../src/imgs"

        self.filter_values["merge"] = st.sidebar.slider('merge',0, 100, 0, 1, '%d')
        self.merge_image = st.sidebar.selectbox(
            "Merge image",
            ([file for file in listdir(mypath) if isfile(join(mypath, file))])
            )

        st.sidebar.header('Convolutional:')

        self.filter_values["blur"] = st.sidebar.slider('Blur',0, 200, 0, 1, '%d')
        self.filter_values["edge"] = st.sidebar.checkbox('Edge', value=False)

        if st.sidebar.button('Apply'): self.run_filters()


    def update_main_layout(self):
        # Main layout
        st.title('Image filter')
        mypath = "../src/imgs"
        st.subheader('Visualizer')

        self.show_images(
            st.multiselect(
                "Select images", 
                ([file for file in listdir(mypath) if isfile(join(mypath, file))])
            )
        )



page = FiltersPage()

