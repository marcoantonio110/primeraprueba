slideshow.module ($Id: README.txt,v 1.6 2006/12/25 16:35:48 timcn Exp $)
================



  === DESCRIPTION ===
slideshow.module provides basic slideshow capabilities. It transforms
images attached to nodes to a JavaScript enabled slideshow. If the user does not
have JavaScript enabled, it degrades to a "regular" slideshow where the "next"
button points to the next image and a whole new page is loaded.
Any post can contain a slideshow: Enable slideshow for a content type
at Administer >> Content management >> Content types >> [content type]



  === FEATURES ===
* Convenient: Just attach an image to the slideshow node
* Lightweight: The JavaScript file weighs only 1.6 kB
* Themable: Just modify three small theme functions
* Usable: Users with JavaScript enabled don't have to reload the page



  === INSTALLATION ===
Just copy the folder to your modules folder and activate the
module in your Drupal installation.

If you do not have enabled the upload module, do it now as this
module (obviously) depends on it.



  === CONFIGURATION ===
Also, you need to configure the upload.module properly. Make sure 
that the path specified unter "administer --> settings --> File system 
settings" exists. The default installation does not come with the 
preconfigured 'files' directory. You have to create that directory 
manually or specify another directory name that exists.

If you don't want to see the files listing below the content of the
slideshow you have to uncheck the checkbox in the "List" column
left to each file in the "Attachments" table.





  === DEVELOPMENT ===
Due to it's simpleness, slideshow.module doesn't provide a bunch
of features. It just does what it's supposed to do. It doesn't have
transitions or a lot of settings you can change.

slideshow.module is in development. There are some features I'd like
to implement that aren't available yet. Planned features include the
possibility to rearrange images, to include images from image nodes,
a more flexible image resize system...

If you have suggestions, don't hesitate to post your suggestions as
an issue on drupal.org. Feel free to supply patches. I'll definitely
review them and roll them in if they are sensible enhancements to
this module.

