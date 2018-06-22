/* test-modulemd-modulestream.c
 *
 * Copyright (C) 2018 Stephen Gallagher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <glib.h>
#include <glib/gstdio.h>
#include <locale.h>

#include "modulemd.h"
#include "private/modulemd-private.h"

typedef struct _StreamFixture
{
} StreamFixture;

static void
convert_from_module_to_modulestream (ModulemdModule *module,
                                     ModulemdModuleStream *dest)
{
  g_assert_true (MODULEMD_IS_MODULE (module));
  g_assert_true (MODULEMD_IS_MODULESTREAM (dest));

  modulemd_modulestream_set_mdversion (
    dest, modulemd_module_peek_mdversion (module));

  modulemd_modulestream_set_arch (dest, modulemd_module_peek_arch (module));

  modulemd_modulestream_set_buildopts (
    dest, modulemd_module_peek_buildopts (module));

  modulemd_modulestream_set_community (
    dest, modulemd_module_peek_community (module));

  modulemd_modulestream_set_content_licenses (
    dest, modulemd_module_peek_content_licenses (module));

  modulemd_modulestream_set_context (dest,
                                     modulemd_module_peek_context (module));

  modulemd_modulestream_set_description (
    dest, modulemd_module_peek_description (module));

  modulemd_modulestream_set_documentation (
    dest, modulemd_module_peek_documentation (module));

  modulemd_modulestream_set_module_components (
    dest, modulemd_module_peek_module_components (module));

  modulemd_modulestream_set_module_licenses (
    dest, modulemd_module_peek_module_licenses (module));

  modulemd_modulestream_set_name (dest, modulemd_module_peek_name (module));

  modulemd_modulestream_set_profiles (dest,
                                      modulemd_module_peek_profiles (module));

  modulemd_modulestream_set_rpm_api (dest,
                                     modulemd_module_peek_rpm_api (module));

  modulemd_modulestream_set_rpm_artifacts (
    dest, modulemd_module_peek_rpm_artifacts (module));

  modulemd_modulestream_set_rpm_components (
    dest, modulemd_module_peek_rpm_components (module));

  modulemd_modulestream_set_rpm_filter (
    dest, modulemd_module_peek_rpm_filter (module));

  modulemd_modulestream_set_servicelevels (
    dest, modulemd_module_peek_servicelevels (module));

  modulemd_modulestream_set_stream (dest,
                                    modulemd_module_peek_stream (module));

  modulemd_modulestream_set_summary (dest,
                                     modulemd_module_peek_summary (module));

  modulemd_modulestream_set_tracker (dest,
                                     modulemd_module_peek_tracker (module));

  modulemd_modulestream_set_version (dest,
                                     modulemd_module_peek_version (module));

  modulemd_modulestream_set_xmd (dest, modulemd_module_peek_xmd (module));


  /* Version-specific content */
  if (modulemd_module_peek_mdversion (module) == MD_VERSION_1)
    {
      modulemd_modulestream_set_buildrequires (
        dest, modulemd_module_peek_buildrequires (module));
      modulemd_modulestream_set_requires (
        dest, modulemd_module_peek_requires (module));
      if (modulemd_module_peek_eol (module))
        {
          modulemd_modulestream_set_eol (dest,
                                         (modulemd_module_peek_eol (module)));
        }
    }
  else if (modulemd_module_peek_mdversion (module) >= MD_VERSION_2)
    {
      modulemd_modulestream_set_dependencies (
        dest, modulemd_module_peek_dependencies (module));
    }
}

static void
modulemd_stream_test_basic (StreamFixture *fixture, gconstpointer user_data)
{
  g_autoptr (ModulemdModuleStream) modulestream = NULL;
  g_autoptr (ModulemdModule) module = NULL;

  /* Properties */
  g_autofree gchar *name = NULL;
  g_autofree gchar *stream = NULL;
  guint64 version;
  g_autofree gchar *context = NULL;
  g_autofree gchar *arch = NULL;
  g_autofree gchar *summary = NULL;
  g_autofree gchar *description = NULL;
  g_autoptr (ModulemdSimpleSet) module_licenses = NULL;
  g_autoptr (ModulemdSimpleSet) content_licenses = NULL;
  g_autoptr (GPtrArray) dependencies = NULL;
  g_autofree gchar *community = NULL;
  g_autofree gchar *documentation = NULL;
  g_autofree gchar *tracker = NULL;
  g_autoptr (ModulemdSimpleSet) rpm_api = NULL;
  g_autoptr (ModulemdSimpleSet) rpm_filter = NULL;
  g_autoptr (ModulemdBuildopts) buildopts = NULL;
  g_autoptr (ModulemdSimpleSet) rpm_artifacts = NULL;

  g_autofree gchar *rpm_macros = NULL;
  g_autoptr (ModulemdSimpleSet) rpm_whitelist = NULL;

  g_autofree gchar *v2_spec_file =
    g_strdup_printf ("%s/spec.v2.yaml", g_getenv ("MESON_SOURCE_ROOT"));

  /* Read in the v2 spec and test its contents */
  module = modulemd_module_new_from_file (v2_spec_file);

  modulestream = modulemd_modulestream_new ();
  g_assert_nonnull (modulestream);
  g_assert_true (MODULEMD_IS_MODULESTREAM (modulestream));

  convert_from_module_to_modulestream (module, modulestream);

  // clang-format off
  g_object_get (modulestream,
                "name", &name,
                "stream", &stream,
                "version", &version,
                "context", &context,
                "arch", &arch,
                "summary", &summary,
                "description", &description,
                "module-licenses", &module_licenses,
                "content-licenses", &content_licenses,
                "dependencies", &dependencies,
                "community", &community,
                "documentation", &documentation,
                "tracker", &tracker,
                "rpm-api", &rpm_api,
                "rpm-filter", &rpm_filter,
                "buildopts", &buildopts,
                "rpm-artifacts", &rpm_artifacts,
                NULL);
  // clang-format on

  g_assert_cmpstr (name, ==, "foo");

  g_assert_cmpstr (stream, ==, "stream-name");

  g_assert_cmpuint (version, ==, 20160927144203);

  g_assert_cmpstr (context, ==, "c0ffee43");

  g_assert_cmpstr (arch, ==, "x86_64");

  g_assert_cmpstr (summary, ==, "An example module");

  g_assert_cmpstr (
    description,
    ==,
    "A module for the demonstration of the metadata format. Also, the "
    "obligatory lorem ipsum dolor sit amet goes right here.");

  g_assert_true (modulemd_simpleset_contains (module_licenses, "MIT"));

  g_assert_true (modulemd_simpleset_contains (content_licenses, "Beerware"));
  g_assert_true (modulemd_simpleset_contains (content_licenses, "GPLv2+"));
  g_assert_true (modulemd_simpleset_contains (content_licenses, "zlib"));

  for (gsize i = 0; i < dependencies->len; i++)
    {
      g_assert_true (
        MODULEMD_IS_DEPENDENCIES (g_ptr_array_index (dependencies, i)));
    }

  g_assert_cmpstr (community, ==, "http://www.example.com/");

  g_assert_cmpstr (documentation, ==, "http://www.example.com/");

  g_assert_cmpstr (tracker, ==, "http://www.example.com/");

  g_assert_true (modulemd_simpleset_contains (rpm_api, "bar"));
  g_assert_true (modulemd_simpleset_contains (rpm_api, "bar-extras"));
  g_assert_true (modulemd_simpleset_contains (rpm_api, "bar-devel"));
  g_assert_true (modulemd_simpleset_contains (rpm_api, "baz"));
  g_assert_true (modulemd_simpleset_contains (rpm_api, "xxx"));

  g_assert_true (modulemd_simpleset_contains (rpm_filter, "baz-nonfoo"));

  g_assert_true (MODULEMD_IS_BUILDOPTS (buildopts));
  rpm_macros = modulemd_buildopts_get_rpm_macros (buildopts);
  g_assert_cmpstr (
    rpm_macros, ==, "%demomacro 1\n%demomacro2 %{demomacro}23\n");
  rpm_whitelist = modulemd_buildopts_get_rpm_whitelist_simpleset (buildopts);
  g_assert_true (modulemd_simpleset_contains (rpm_whitelist, "fooscl-1-bar"));
  g_assert_true (modulemd_simpleset_contains (rpm_whitelist, "fooscl-1-baz"));
  g_assert_true (modulemd_simpleset_contains (rpm_whitelist, "xxx"));
  g_assert_true (modulemd_simpleset_contains (rpm_whitelist, "xyz"));

  g_assert_true (MODULEMD_IS_SIMPLESET (rpm_artifacts));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "bar-0:1.23-1.module_deadbeef.x86_64"));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "bar-devel-0:1.23-1.module_deadbeef.x86_64"));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "bar-extras-0:1.23-1.module_deadbeef.x86_64"));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "baz-0:42-42.module_deadbeef.x86_64"));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "xxx-0:1-1.module_deadbeef.x86_64"));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "xxx-0:1-1.module_deadbeef.i686"));
  g_assert_true (modulemd_simpleset_contains (
    rpm_artifacts, "xyz-0:1-1.module_deadbeef.x86_64"));
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);
  g_test_bug_base ("https://bugzilla.redhat.com/show_bug.cgi?id=");


  g_test_add ("/modulemd/modulestream/init",
              StreamFixture,
              NULL,
              NULL,
              modulemd_stream_test_basic,
              NULL);

  return g_test_run ();
};
