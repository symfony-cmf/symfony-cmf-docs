elFinder
========

The media browser `elFinder`_ is integrated with Symfony using the
`FMElfinderBundle`_. The MediaBundle provides an adapter to use it with objects
implementing the MediaBundle interfaces.

.. note::

    The MediaBundle elFinder adapter is currently only implemented for Doctrine
    PHPCR-ODM.

Installation
~~~~~~~~~~~~

1. Install the FMElfinderBundle according to the `FMElfinderBundle documentation`_.
2. Configure the FMElfinderBundle to use the MediaBundle adapter:

   .. configuration-block::

       .. code-block:: yaml

           # app/config/config.yml
           fm_elfinder:
               locale: "%locale%"
               editor: ckeditor
               connector:
                   roots:
                       media:
                           driver: cmf_media.adapter.elfinder.phpcr_driver
                           path: "%cmf_media.persistence.phpcr.media_basepath%"
                           upload_allow: ['all']
                           upload_max_size: 2M

       .. code-block:: xml

           <!-- app/config/config.xml -->
           <?xml version="1.0" charset="UTF-8" ?>
           <container xmlns="http://symfony.com/schema/dic/services">

                <config xmlns="http://example.org/dic/schema/fm_elfinder"
                    locale="%locale%"
                    editor="ckeditor"
                >
                    <connector>
                        <root
                            name="media"
                            driver="cmf_media.adapter.elfinder.phpcr_driver"
                            path="%cmf_media.persistence.phpcr.media_basepath%"
                            upload-max-size="2M"
                            upload-allow="all"
                        />
                    </connector>
                </config>

           </container>

       .. code-block:: php

           // app/config/config.php
           $container->loadFromExtension('fm_elfinder', array(
               'locale' => '%locale%',
               'editor' => 'ckeditor',
               'connector' => array(
                   'roots' => array(
                       'media' => array(
                           'driver' => 'cmf_media.adapter.elfinder.phpcr_driver',
                           'path' => '%cmf_media.persistence.phpcr.media_basepath%',
                           'upload_allow': array('all'),
                           'upload_max_size' => '2M',
                       ),
                   ),
               ),
           ));

.. note::

    The driver service depends on your storage layer. For now, the MediaBundle
    only provides a PHPCR driver. To make the above configuration work, you
    need to enable PHPCR either globally on ``cmf_core.persistence:phpcr`` or,
    if you only use the MediaBundle, directly on the bundle at
    ``cmf_media.persistence.phpcr``.

3. When using the LiipImagineBundle, add an imagine filter for the thumbnails:

   .. configuration-block::

       .. code-block:: yaml

           # app/config/config.yml
           liip_imagine:
               # ...
               filter_sets:
                   # default filter to be used for elfinder thumbnails
                   elfinder_thumbnail:
                       data_loader: cmf_media_doctrine_phpcr
                       quality: 85
                       filters:
                           thumbnail: { size: [48, 48], mode: inset }
                   # ...

       .. code-block:: xml

           <!-- app/config/config.xml -->
           <?xml version="1.0" charset="UTF-8" ?>
           <container xmlns="http://symfony.com/schema/dic/services">

                <config xmlns="http://example.org/dic/schema/liip_imagine">
                    <!-- ... -->
                    <!-- default filter to be used for elfinder thumbnails -->
                    <filter-set name="elfinder_thumbnail" data-loader="cmf_media_doctrine_phpcr" quality="85">
                        <filter name="thumbnail" size="48,48" mode="inset"/>
                    </filter-set>
                    <!-- ... -->
                </config>

           </container>

       .. code-block:: php

           // app/config/config.php
           $container->loadFromExtension('liip_imagine', array(
               // ...
               'filter_sets' => array(
                   // default filter to be used for elfinder thumbnails
                   'elfinder_thumbnail' => array(
                       'data_loader' => 'cmf_media_doctrine_phpcr',
                       'quality'     => 85,
                       'filters'     => array(
                           'thumbnail' => array(
                               'size' => array(48, 48),
                               'mode' => 'inset',
                           ),
                       ),
                   ),
                   // ...
               ),
           ));

4. Test the elFinder browser by navigating to: http://<yoursite>/app_dev.php/elfinder

.. _`elFinder`: http://elfinder.org
.. _`FMElfinderBundle`: https://github.com/helios-ag/FMElfinderBundle
.. _`FMElfinderBundle documentation`: https://github.com/helios-ag/FMElfinderBundle#readme
