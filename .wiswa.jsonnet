local utils = import 'utils.libjsonnet';

(import 'defaults.libjsonnet') + {
  local top = self,
  // General settings

  // Shared
  github_username: 'Tatsh',
  security_policy_supported_versions: { '0.0.x': ':white_check_mark:' },
  authors: [
    {
      'family-names': 'Udvare',
      'given-names': 'Andrew',
      email: 'audvare@gmail.com',
      name: '%s %s' % [self['given-names'], self['family-names']],
    },
  ],
  project_name: 'winpbcopy',
  version: '0.0.2',
  description: 'pbcopy/paste for Windows.',
  keywords: ['kate', 'kde', 'plasma', 'wakatime'],
  want_main: false,
  copilot: {
    intro: "winpbcopy simulates macOS' pbcopy/pbpaste commands.",
  },
  social+: {
    mastodon+: { id: '109370961877277568' },
  },

  // GitHub
  github+: {
    funding+: {
      ko_fi: 'tatsh2',
      liberapay: 'tatsh2',
      patreon: 'tatsh2',
    },
  },

  // C++ only
  cmake+: {
  },
  project_type: 'c',
  vcpkg+: {
    dependencies: [{
      name: 'ecm',
      'version>=': '6.7.0',
    }],
  },
}
