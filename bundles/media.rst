.. index::
    single: Media; Bundles
    single: MediaBundle

MediaBundle
===========

The `MediaBundle`_ provides a way to store and edit any media and provides a
generic base of common interfaces and models that allow the user to build media
management solutions for a CMS. Media can be images, binary documents like pdf,
embedded movies, uploaded movies, mp3s, whatever. The implementation of this
bundle is **very** minimalistic and is focused on images and download files,
going any further is duplicating the `SonataMediaBundle`_.

This bundle provides:

* base documents for a simple model;
* base FormTypes for the simple model;
* a download controller for file downloads;
* and handlers for common processes related to the storage layer.

It can also provide adapters for integrating:

* media browsers (elFinder, ckFinder, MceFileManager, whatever);
* image manipulation (Imagine, LiipImagineBundle).

Because interfaces are used, it should not matter whether an object is
persisted using Doctrine PHPCR-ODM, Doctrine ORM or something else.

.. note::

    The bundle is build to support several persistance layers. For version 1.0
    Doctrine PHPCR-ODM is implemented.

.. index:: MediaBundle, PHPCR, ODM, ORM

Dependencies
------------

For PHPCR:

* :doc:`PHPCR-ODM <phpcr_odm>` is used to persist the bundles documents.

When using the Gaufrette adapter:

* `KnpLabs/Gaufrette`_;
* `phpcr/phpcr-utils`_ (PHPCR).

For web editing tools:

* TODO

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_media:
            manager_registry: doctrine_phpcr # or doctrine_orm
            manager_name:     default
            media_basepath:   /cms/media
            media_class:      ~
            file_class:       ~
            directory_class:  ~
            image_class:      ~
            use_liip_imagine: 'auto'
            imagine_filter:   image_upload_thumbnail
            extra_filters:
                - imagine_filter_name1
                - imagine_filter_name2

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/media"
            manager-registry="doctrine_phpcr"
            manager-name="default"
            media-basepath="/cms/media"
            media-class=""
            file-class=""
            directory-class=""
            image-class=""
            use-liip-imagine="auto"
            imagine-filter="image_upload_thumbnail"
        >
            <extra-filter>imagine_filter_name1</extra-filter>
            <extra-filter>imagine_filter_name2</extra-filter>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_media', array(
            'manager_registry' => 'doctrine_phpcr', // or doctrine_orm
            'manager_name'     => 'default',
            'media_basepath'   => '/cms/media',
            'media_class'      => null,
            'file_class'       => null,
            'directory_class'  => null,
            'image_class'      => null,
            'use_liip_imagine' => 'auto',
            'imagine_filter'   => 'image_upload_thumbnail',
            'extra_filters'    => array(
                'imagine_filter_name1',
                'imagine_filter_name2',
            ),
        ));

Installation
------------

When using the download controller add the following lines to the end of your
routing file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml

        # ...
        media:
            resource: "@CmfMediaBundle/Resources/config/routing/download.xml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

           <!-- ... -->

           <import resource="@CmfMediaBundle/Resources/config/routing/download.xml" />
       </routes>

   .. code-block:: php

       // app/config/routing.php
       $collection->addCollection(
           $loader->import("@CmfMediaBundle/Resources/config/routing/download.xml")
       );

       return $collection;

Interfaces
----------

Using the interfaces leaves it open to use separate classes for each media type
or one media class for all. Also how data is persisted can be changed depending
on the situation, an example is that a file size can be persisted in the
metadata but also can have it's own field. This way a project can start with an
Image class and later grow.

The MediaBundle provides the following interfaces:

* **MediaInterface**:      base class;
* **MetaDataInterface**:   meta data definition;
* **FileInterface**:       identifies a file;
* **ImageInterface**:      identifies the media as an image;
* **FileSystemInterface**: the file is stored on a filesystem and the path is
  persisted in the media object;
* **BinaryInterface**:     mostly used when the file is stored in the media
  object;
* **DirectoryInterface**:  identifies a directory;
* **HierarchyInterface**:  media objects containing directories, the path to
  a media is: ``/path/to/file/filename.ext``.

TODO: add interfaces diagram

.. image:: ../../_images/bundles/media_interfaces.png
   :align: center

.. note::

    When it is possible to use the interfaces to build features these become
    independent of the persistance layer. And you can integrate the features
    with other implementations using the Symfony CMF MediaBundle interfaces.

Form Types
----------

The bundle provides a couple of handy form types along with form data
transformers.

cmf_media_image
~~~~~~~~~~~~~~~

The ``cmf_media_image`` form maps to an object that implements the
``Symfony\Cmf\Bundle\MediaBundle\ImageInterface`` and provides a preview of the
uploaded image. To use it, you need to include the `LiipImagineBundle`_ in your
project and define an imagine filter for thumbnails.

You can configure the imagine filter to use for the preview, as well as
additional filters to remove from cache when the image is replaced. If the
filter is not specified, it defaults to ``image_upload_thumbnail``.

.. configuration-block::

    .. code-block:: yaml

        # Imagine Configuration
        liip_imagine:
            # ...
            filter_sets:
                # define the filter to be used with the image preview
                image_upload_thumbnail:
                    data_loader: cmf_media_doctrine_phpcr
                    filters:
                        thumbnail: { size: [100, 100], mode: outbound }

Then you can add images to document forms as follows::

    use Symfony\Component\Form\FormBuilderInterface;

    protected function configureFormFields(FormBuilderInterface $formBuilder)
    {
         $formBuilder
            ->add('image', 'cmf_media_image', array('required' => false))
         ;
    }

.. tip::

   If you set required to true for the image, the user must re-upload a new
   image each time he edits the form. If the document must have an image, it
   makes sense to require the field when creating a new document, but make it
   optional when editing an existing document. We are
   `trying to make this automatic`_.

Next you will need to add the ``fields.html.twig`` template from the
MediaBundle to the ``form.resources``, to actually see the preview of the
uploaded image in the backend.

.. configuration-block::

    .. code-block:: yaml

        # Twig Configuration
        twig:
            form:
                resources:
                    - 'CmfMediaBundle:Form:fields.html.twig'

The document that should contain the Image document has to implement a setter
method.  To profit from the automatic guesser of the form layer, the name in
the form element and this method name have to match::

    public function setImage(ImageInterface $image = null)
    {
        if (!$image) {
            return;
        } elseif ($this->image) {
            // TODO: https://github.com/doctrine/phpcr-odm/pull/262
            $this->image->copyContentFromFile($image);
        } elseif ($image instanceof Image) {
            $this->image = $image;
        } else {
            $this->image = new Image;
            $this->image->copyContentFromFile($image);
        }
    }

To delete an image, you need to delete the document containing the image.
(There is a proposal to improve the user experience for that in a
`DoctrinePHPCRBundle issue`_.)

.. note::

    There is a doctrine listener to invalidate the imagine cache for the
    filters you specified. This listener will only operate when an Image is
    changed in a web request, but not when a CLI command changes images. When
    changing images with commands, you should handle cache invalidation in the
    command or manually remove the imagine cache afterwards.

Templating
----------

The media bundle contains a Twig extension, it contains the following functions:

* **cmf_media_download_url**: returns the url to download a media implementing
  the FileInterface

  .. configuration-block::

      .. code-block:: jinja

          <a href="{{ cmf_media_download_url(file) }}" title="Download">Download</a>

      .. code-block:: php+html

          <a href="<?php echo $view['cmf_media']->downloadUrl($file) ?>" title="Download">Download</a>

SonataMediaBundle integration
-----------------------------

If you want to have more advanced features you can use the `SonataMediaBundle`_.
The MediaBundle is build to be fully compatible with the SonataMediaBundle.

.. note::

    For version 1.1 the integration with the SonataMediaBundle is planned, and
    if possible an upgrade command and documentation is added.

Web editing tools
-----------------

The MediaBundle provides integration with WYSIWYG editors and
:doc:`Create <create>`. Media support is mostly split in:

* uploading media
* browsing and selecting media

Ckeditor
~~~~~~~~

TODO - sync documentation with:
- the CreateBundle
- the tutorial "Installing and Configuring Inline Editing"
- and maybe also with the tutorial "Creating a CMS using CMF and Sonata"

Configuration
.............

TODO

Installation
............

TODO

Adapters
--------

The MediaBundle provides some adapters for integrating media with php libraries
and Symfony bundles.

LiipImagine
~~~~~~~~~~~

TODO

Gaufrette
~~~~~~~~~

TODO

Elfinder
~~~~~~~~

TODO

.. _`MediaBundle`: https://github.com/symfony-cmf/MediaBundle#readme
.. _`LiipImagineBundle`: https://github.com/liip/LiipImagineBundle
.. _`trying to make this automatic`: https://groups.google.com/forum/?fromgroups=#!topic/symfony2/CrooBoaAlO4
.. _`DoctrinePHPCRBundle issue`: https://github.com/doctrine/DoctrinePHPCRBundle/issues/40
.. _`KnpLabs/Gaufrette`: https://github.com/KnpLabs/Gaufrette
.. _`phpcr/phpcr-utils`: https://github.com/phpcr/phpcr-utils
.. _`SonataMediaBundle`: https://github.com/sonata-project/SonataMediaBundle