.. index::
    single: Media; Bundles
    single: MediaBundle

MediaBundle
===========

    The `MediaBundle`_ provides a way to store and edit any media and provides
    a generic base of common interfaces and models that allow the user to build
    media management solutions for a CMS.

Media can be images, binary documents (like pdf files), embedded movies,
uploaded movies, MP3s, etc. The implementation of this bundle is **very**
minimalistic and is focused on images and download files. If you need more
functionality (like cdn, thumbnail generation, providers for different media
types and more) take a look at `SonataMediaBundle`_. The MediaBundle provides
integration with SonataMediaBundle.

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

Installation
------------

1. You can install the bundle in 2 different ways:

  * Use the official Git repository (https://github.com/symfony-cmf/MediaBundle);
  * Install it via Composer (``symfony-cmf/media-bundle`` on `Packagist`_).

2. When using the file and image controller for downloading, uploading and
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

3. For now, the only supported persistence layer is PHPCR. If you enabled PHPCR
   on the CoreBundle, you need to do nothing here. If you do not have the
   CMF CoreBundle in your project, you need to configure
   ``cmf_media.persistence.phpcr.enabled: true``.

4. For PHPCR, run the ``doctrine:phpcr:repository:init`` command, to have the
   base paths initialized, using the
   :ref:`repository initializers <phpcr-odm-repository-initializers>`.

Interfaces
----------

Using the interfaces leaves it open to use separate classes for each media type
or one media class for all. Also how data is persisted can be changed depending
on the situation, an example is that a file size can be persisted in the
metadata but also can have its own field. This way a project can start with an
``Image`` class and later grow.

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

For instance, take an image. In general you know how to store images and how
to organize them in a folder, maybe you added some subfolders and then the
images.

On a Windows machine the image file has a **path** that can look like this:
``C:\path\to\my\image.jpg``

On a Linux or Mac OSX machine the image file has a **path** that can look like
this: ``/path/to/my/image.jpg``

The above paths are called a filesystem path and contain the following
information:

* the name of the file: ``image.jpg``;
* the path to the subfolder is ``/path/to/my``, the folder above has
  ``/path/to`` as path, etc.

As you can see, the path contains both information about the file and about
its parents.

The MediaBundle re-used the **path** idea to make media objects unique and to
be able to get the parent from it when needed:

* parents are always separated by a ``/``: ``/path``, ``/path/to`` and
  ``/path/to/my`` are all parents;
* parents are combined with the media name after the last "/": ``image.jpg``;
* parents always start with a ``/``.

PHPCR also uses the **path** in a similar way, for PHPCR the path is also used
as media object id. For ORM or ODM storage, the id is more likely a number.

If you look at the ``MediaManagerInterface`` you will see several methods
using this **path**:

* **getPath**: allows you to create a path for a media object stored
  in Phpcr, ORM or another Doctrine storage;
* **getUrlSafePath**: transforms the path to be safe for usage in an url;
* **mapPathToId**: transforms the path back to a media object id so it can be
  looked up in the Doctrine store;
* **mapUrlSafePathToId**: transforms an url safe path directly back to an id.

Form Types
----------

The MediaBundle provides some usefull form types, read more about the types in
:doc:`form_types`.

Templating
----------

The media bundle contains a Twig extension, it contains the following functions:

* **cmf_media_download_url**: returns the url to download a media implementing
  the FileInterface

  .. configuration-block::

      .. code-block:: html+jinja

          <a href="{{ cmf_media_download_url(file) }}" title="Download">Download</a>

      .. code-block:: html+php

          <a href="<?php echo $view['cmf_media']->downloadUrl($file) ?>" title="Download">Download</a>

* **cmf_media_display_url**: returns the url to display a media implementing
  the ImageInterface

  .. configuration-block::

      .. code-block:: html+jinja

          <img src="{{ cmf_media_display_url(image) }}" alt="" />

      .. code-block:: html+php

          <img src="<?php echo $view['cmf_media']->displayUrl($image) ?>" alt="" />

SonataMediaBundle Integration
-----------------------------

If you want to have more advanced features you can use the `SonataMediaBundle`_.
The MediaBundle is built to be fully compatible with the SonataMediaBundle.

.. note::

    For version 1.1 the integration with the SonataMediaBundle is planned, and
    - if possible - an upgrade command and documentation is added.

Web Editing Tools
-----------------

The MediaBundle provides integration with WYSIWYG editors and
:doc:`Create <../create/introduction>`. Media support is mostly split in:

* `Uploading Files`_
* `browsing and Selecting Media`_

Uploading Files
~~~~~~~~~~~~~~~

The file and image controller of the MediaBundle provide an upload action, it
uses an ``UploadFileHelperInterface`` instance. If you want to make your own upload
implementation you can use the ``cmf_media.upload_file_helper`` or
``cmf_media.upload_image_helper`` service directly. The default upload action
is protected by the ``ROLE_CAN_UPLOAD_FILE`` role.

The ``UploadFileHelper`` contains ``UploadEditorHelperInterface`` instances.
This handles the response returned of the file upload depending on the web
editing tool used and can be json, JavaScript or something else. Implement
your own for specific needs, add it to the service configuration and tag the
service with ``cmf_media.upload_editor_helper``, the tag alias is the editor
helper name. The ``UploadFileHelper`` checks the request for the parameter
``editor`` to select the requested ``UploadEditorHelperInterface`` to create
the response.

Browsing and Selecting Media
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When a file, image or other media has to be inserted in a WYSIWYG editor the
user first has to browse and find the media. A media browser is a separate
tool that can be integrated with the WYSIWYG editor and assists the user with
this task.

Adapters
--------

The MediaBundle provides some adapters for integrating media with php libraries
and Symfony bundles:

* :doc:`adapters/liip_imagine`
* :doc:`adapters/elfinder`
* :doc:`adapters/gaufrette`

.. _`MediaBundle`: https://github.com/symfony-cmf/MediaBundle#readme
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/media-bundle
.. _`KnpLabs/Gaufrette`: https://github.com/KnpLabs/Gaufrette
.. _`phpcr/phpcr-utils`: https://github.com/phpcr/phpcr-utils
.. _`jms/serializer-bundle`: https://github.com/schmittjoh/JMSSerializerBundle
.. _`SonataMediaBundle`: https://github.com/sonata-project/SonataMediaBundle
