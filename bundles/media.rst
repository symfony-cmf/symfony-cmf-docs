.. index::
    single: Media; Bundles
    single: MediaBundle

MediaBundle
===========

The `MediaBundle`_ provides a way to store and edit any media and provides a
generic base of common interfaces and models that allow the user to build media
management solutions for a CMS. Media can be images, binary documents (like pdf
files), embedded movies, uploaded movies, MP3s, etc. The implementation of this
bundle is **very** minimalistic and is focused on images and download files.
If you need more functionality (like cdn, thumbnail generation, providers for
different media types and more) take a look at `SonataMediaBundle`_. The
MediaBundle provides integration with SonataMediaBundle.

This bundle provides:

* base documents for a simple model;
* base FormTypes for the simple model;
* a file controller for file downloads and uploads;
* an upload file helper that abstracts the uploads;
* and an image controller to display an image.

It can also provide adapters and helpers for integrating:

* media browsers (elFinder, ckFinder, MceFileManager, whatever);
* image manipulation (Imagine, LiipImagineBundle).

Because interfaces are used, it should not matter whether an object is
persisted using Doctrine PHPCR-ODM, Doctrine ORM or something else.

.. note::

    The bundle is built to support several persistence layers. Only the
    Doctrine PHPCR-ODM is implemented in version 1.0.

.. index:: MediaBundle, PHPCR, ODM, ORM

Dependencies
------------

For PHPCR:

* :doc:`PHPCR-ODM <phpcr_odm>` is used to persist the bundles documents;
* `phpcr/phpcr-utils`_.

When using the Gaufrette adapter:

* `KnpLabs/Gaufrette`_.

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_media:
            persistence:
                phpcr:
                    enabled:         true
                    manager_name:    ~
                    media_basepath:  /cms/media
                    media_class:     ~
                    file_class:      ~
                    directory_class: ~
                    image_class:     ~
            upload_file_role:   ROLE_CAN_UPLOAD_FILE
            use_jms_serializer: auto
            use_imagine:        auto
            imagine_filter:     image_upload_thumbnail
            extra_filters:
                - imagine_filter_name1
                - imagine_filter_name2

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/media"
            upload-file-role="ROLE_CAN_UPLOAD_FILE"
            use-jms-serializer="auto"
            use-imagine="auto"
            imagine-filter="image_upload_thumbnail"
        >
            <persistence>
                <phpcr
                    enabled="true"
                    manager-name=""
                    media-basepath="/cms/media"
                    media-class=""
                    file-class=""
                    directory-class=""
                    image-class=""
                />
            </persistence>
            <extra-filter>imagine_filter_name1</extra-filter>
            <extra-filter>imagine_filter_name2</extra-filter>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_media', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled'         => true,
                    'manager_name'    => null,
                    'media_basepath'  => '/cms/media',
                    'media_class'     => null,
                    'file_class'      => null,
                    'directory_class' => null,
                    'image_class'     => null,
                ),
             ),
            'upload_file_role'   => 'ROLE_CAN_UPLOAD_FILE',
            'use_jms_serializer' => 'auto',
            'use_imagine'        => 'auto',
            'imagine_filter'     => 'image_upload_thumbnail',
            'extra_filters'      => array(
                'imagine_filter_name1',
                'imagine_filter_name2',
            ),
        ));

Installation
------------

1. When using the file and image controller for downloading, uploading and
   displaying, add the following lines to the end of your routing file:

   .. configuration-block::

       .. code-block:: yaml

           # app/config/routing.yml

           # ...
           cmf_media_file:
               resource: "@CmfMediaBundle/Resources/config/routing/file.xml"

           cmf_media_image:
               resource: "@CmfMediaBundle/Resources/config/routing/image.xml"

       .. code-block:: xml

           <!-- app/config/routing.xml -->
           <?xml version="1.0" encoding="UTF-8" ?>
           <routes xmlns="http://symfony.com/schema/routing"
               xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
               xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

              <!-- ... -->

              <import resource="@CmfMediaBundle/Resources/config/routing/file.xml" />
              <import resource="@CmfMediaBundle/Resources/config/routing/image.xml" />
           </routes>

       .. code-block:: php

           // app/config/routing.php
           $collection->addCollection(
               $loader->import("@CmfMediaBundle/Resources/config/routing/file.xml")
           );
           $collection->addCollection(
               $loader->import("@CmfMediaBundle/Resources/config/routing/image.xml")
           );

           return $collection;

2. Run the ``doctrine:phpcr:repository:init`` command, it runs all tagged
   :ref:`phpcr-odm-repository-initializers` including the MediaBundle
   initializer.

Interfaces
----------

Using the interfaces leaves it open to use separate classes for each media type
or one media class for all. Also how data is persisted can be changed depending
on the situation, an example is that a file size can be persisted in the
metadata but also can have it's own field. This way a project can start with an
Image class and later grow.

The MediaBundle provides the following interfaces:

* **MediaInterface**:      base class;
* **MetadataInterface**:   meta data definition;
* **FileInterface**:       identifies a file;
* **ImageInterface**:      identifies the media as an image;
* **FileSystemInterface**: the file is stored on a filesystem and the path is
  persisted in the media object;
* **BinaryInterface**:     mostly used when the file is stored in the media
  object;
* **DirectoryInterface**:  identifies a directory;
* **HierarchyInterface**:  media objects containing directories, the path to
  a media is: ``/path/to/file/filename.ext``.

.. note::

    When it is possible to use the interfaces to build features these become
    independent of the persistence layer. And you can integrate the features
    with other implementations using the Symfony CMF MediaBundle interfaces.

Terminology
-----------

The MediaBundle provides a generic base of common models to work with media.
When working with them several terms can be used.

Let's take the example of an image. In general we know how to store images,
we organize them in a folder, maybe we added some subfolders and then the
images.

On a windows machine the image file has a **path** that can look like this:
``C:\path\to\my\image.jpg``

On a linux or Mac OSX machine the image file has a **path** that can look like
this: ``/path/to/my/image.jpg``

Above paths are called a filesystem path and contain the following information:

* the name of the file: *image.jpg*
* and the path to the subfolder is ``/path/to/my``, the folder above has
  ``/path/to`` as path, etc.

What we see is that a path contains both information about the file and its
parents.

For the MediaBundle we re-use the **path** idea to make media objects unique
and be able to get the parent from it when needed:

* parents are always separated by a "/": ``/path``, ``/path/to`` and
  ``/path/to/my`` are all parents;
* and combined with the media name after the last "/": ``image.jpg``;
* it also always starts with a "/".

Phpcr also uses the **path** in a similar way, for Phpcr the path is also used
as media object id. For ORM or ODM storage the id is more likely a number.

If you look at the MediaManagerInterface you will see several methods using
this **path**:

* **getPath**: allows you to create a path for a media object stored
  in Phpcr, ORM or another Doctrine storage;
* **getUrlSafePath**: transforms the path to be safe for usage in an url;
* **mapPathToId**: transforms the path back to a media object id so it can be
  looked up in the Doctrine store;
* **mapUrlSafePathToId**: transforms an url safe path directly back to an id.

Form Types
----------

The bundle provides a couple of handy form types along with form data
transformers.

cmf_media_image
~~~~~~~~~~~~~~~

The ``cmf_media_image`` form maps to an object that implements the
``Symfony\Cmf\Bundle\MediaBundle\ImageInterface`` and provides a preview of the
uploaded image.

if `LiipImagineBundle`_ is used in your project you can configure the imagine
filter to use for the preview, as well as additional filters to remove from
cache when the image is replaced. If the filter is not specified, it defaults
to ``image_upload_thumbnail``.

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
            $this->image = new Image();
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

      .. code-block:: html+php

          <a href="<?php echo $view['cmf_media']->downloadUrl($file) ?>" title="Download">Download</a>

* **cmf_media_display_url**: returns the url to display a media implementing
  the ImageInterface

  .. configuration-block::

      .. code-block:: jinja

          <img src="{{ cmf_media_display_url(image) }}" alt="" />

      .. code-block:: html+php

          <img src="<?php echo $view['cmf_media']->displayUrl($image) ?>" alt="" />

SonataMediaBundle integration
-----------------------------

If you want to have more advanced features you can use the `SonataMediaBundle`_.
The MediaBundle is built to be fully compatible with the SonataMediaBundle.

.. note::

    For version 1.1 the integration with the SonataMediaBundle is planned, and
    if possible an upgrade command and documentation is added.

Web editing tools
-----------------

The MediaBundle provides integration with WYSIWYG editors and
:doc:`Create <create>`. Media support is mostly split in:

* uploading a file
* browsing and selecting media

Uploading files
~~~~~~~~~~~~~~~

The file and image controller of the MediaBundle provide an upload action, it
uses the ``UploadFileHelper``. If you want to make your own upload
implementation you can use the ``UploadFileHelper`` directly. The default
upload action is protected by the ``ROLE_CAN_UPLOAD_FILE`` role.

The ``UploadFileHelper`` contains ``UploadEditorHelperInterface`` instances.
This handles the response returned of the file upload depending on the web
editing tool used and can be json, javascript or something else. Implement
your own for specific needs, add it to the service configuration and tag the
service with ``cmf_media.upload_editor_helper``, the tag alias is the editor
helper name. The ``UploadFileHelper`` checks the request for the parameter
``editor`` to select the requested ``UploadEditorHelperInterface`` to create
the response.

Adapters
--------

The MediaBundle provides some adapters for integrating media with php libraries
and Symfony bundles.

LiipImagine
~~~~~~~~~~~

For LiipImagine a data loader is included:
``Symfony\Cmf\Bundle\MediaBundle\Adapter\LiipImagine\CmfMediaDoctrineLoader''.
It will work for all image object implementing
``Symfony\Cmf\Bundle\MediaBundle\ImageInterface`` and is automatically enabled
if the LiipImagineBundle is installed.

The dataloader has the name: ``cmf_media_doctrine_phpcr``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        liip_imagine:
            # ...
            filter_sets:
                # default filter to be used with the image preview
                image_upload_thumbnail:
                    data_loader: cmf_media_doctrine_phpcr
                    quality: 85
                    filters:
                        thumbnail: { size: [100, 100], mode: outbound }
                # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/dic/schema/liip_imagine">
                <!-- ... -->
                <!-- default filter to be used with the image preview -->
                <filter-set name="image_upload_thumbnail" data-loader="cmf_media_doctrine_phpcr" quality="85">
                    <filter name="thumbnail" size="100,100" mode="outbound"/>
                </filter-set>
                <!-- ... -->
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('liip_imagine', array(
            // ...
            'filter_sets' => array(
                // default filter to be used with the image preview
                'image_upload_thumbnail' => array(
                    'data_loader' => 'cmf_media_doctrine_phpcr',
                    'quality'     => 85,
                    'filters'     => array(
                        'thumbnail' => array(
                            'size' => array(616, 419),
                            'mode' => 'outbound',
                        ),
                    ),
                ),
                // ...
            ),
        ));

Gaufrette
~~~~~~~~~

The Gaufrette adapter currently only has a Phpcr version:
``Symfony\Cmf\Bundle\MediaBundle\Gaufrette\Adapter\PhpcrCmfMediaDoctrine``.

.. _`MediaBundle`: https://github.com/symfony-cmf/MediaBundle#readme
.. _`LiipImagineBundle`: https://github.com/liip/LiipImagineBundle
.. _`trying to make this automatic`: https://groups.google.com/forum/?fromgroups=#!topic/symfony2/CrooBoaAlO4
.. _`DoctrinePHPCRBundle issue`: https://github.com/doctrine/DoctrinePHPCRBundle/issues/40
.. _`KnpLabs/Gaufrette`: https://github.com/KnpLabs/Gaufrette
.. _`phpcr/phpcr-utils`: https://github.com/phpcr/phpcr-utils
.. _`SonataMediaBundle`: https://github.com/sonata-project/SonataMediaBundle