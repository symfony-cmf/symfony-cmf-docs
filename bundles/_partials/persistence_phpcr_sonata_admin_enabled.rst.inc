**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes are activated. If set to ``auto``, they are
activated only if the SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.
