"use client";

import { useState, useEffect } from "react";
import { Shield, Send, Server, Lock, Eye, Zap } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Card, CardHeader, CardTitle, CardDescription, CardContent, CardFooter } from "@/components/ui/card";

export default function Component() {
  const [selectedServer, setSelectedServer] = useState("");
  const [message, setMessage] = useState("");
  const [response, setResponse] = useState("");
  const [scrollPosition, setScrollPosition] = useState(0);
  const [showNavbar, setShowNavbar] = useState(false);

  const servers = [
    { id: "server1", name: "Server 1 - Fast & Secure" },
    { id: "server2", name: "Server 2 - Ultra Private" },
    { id: "server3", name: "Server 3 - Balanced" },
  ];

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setResponse(`Message sent through ${selectedServer}: "${message}"`);
    setMessage("");
  };

  useEffect(() => {
    const handleScroll = () => {
      const position = window.scrollY;
      setScrollPosition(position);
      setShowNavbar(position > 100);
    };

    window.addEventListener("scroll", handleScroll, { passive: true });

    return () => {
      window.removeEventListener("scroll", handleScroll);
    };
  }, []);

  const backgroundStyle = {
    backgroundColor: `rgb(88, 28, 135, ${Math.min(scrollPosition / 500, 1)})`,
  };

  return (
    <div
      className="min-h-screen text-white transition-colors duration-500 ease-in-out bg-black"
      style={backgroundStyle}
    >
      <header
        className={`fixed top-0 left-0 right-0 z-10 bg-black bg-opacity-50 py-4 transition-opacity duration-300 ${
          showNavbar ? "opacity-100" : "opacity-0 pointer-events-none"
        }`}
      >
        <div className="container mx-auto px-4 flex justify-between items-center">
          <div className="flex items-center space-x-2">
            <Shield className="w-8 h-8" />
            <span className="text-2xl font-bold">SecureProxy</span>
          </div>
          <nav>
            <ul className="flex space-x-4">
              <li>
                <a href="#" className="hover:text-purple-300">
                  Home
                </a>
              </li>
              <li>
                <a href="#" className="hover:text-purple-300">
                  About
                </a>
              </li>
              <li>
                <a href="#" className="hover:text-purple-300">
                  Contact
                </a>
              </li>
            </ul>
          </nav>
        </div>
      </header>

      <main className="container mx-auto px-4 py-20">
        <section className="min-h-screen flex items-center justify-center">
          <div className="text-center space-y-8">
            <h1 className="text-5xl font-bold mb-4">Why Choose SecureProxy?</h1>
            <div className="grid gap-8 md:grid-cols-3">
              <Card className="bg-purple-900 bg-opacity-50 border-purple-600">
                <CardContent className="flex flex-col items-center space-y-4 p-6">
                  <Lock className="w-12 h-12 text-purple-400" />
                  <h3 className="text-xl font-semibold">Enhanced Privacy</h3>
                  <p className="text-purple-300">Your data is encrypted and your identity is protected.</p>
                </CardContent>
              </Card>
              <Card className="bg-purple-900 bg-opacity-50 border-purple-600">
                <CardContent className="flex flex-col items-center space-y-4 p-6">
                  <Zap className="w-12 h-12 text-purple-400" />
                  <h3 className="text-xl font-semibold">Lightning Fast</h3>
                  <p className="text-purple-300">Our servers ensure quick and efficient message delivery.</p>
                </CardContent>
              </Card>
              <Card className="bg-purple-900 bg-opacity-50 border-purple-600">
                <CardContent className="flex flex-col items-center space-y-4 p-6">
                  <Eye className="w-12 h-12 text-purple-400" />
                  <h3 className="text-xl font-semibold">No Logging Policy</h3>
                  <p className="text-purple-300">We dont keep any logs of your activities or messages.</p>
                </CardContent>
              </Card>
            </div>
          </div>
        </section>

        <section className="min-h-screen flex items-center justify-center">
          <Card className="w-full max-w-md bg-purple-800 border-purple-600">
            <CardHeader>
              <CardTitle className="text-2xl font-bold flex items-center gap-2">
                <Shield className="w-6 h-6" /> Forward Proxy Server
              </CardTitle>
              <CardDescription className="text-purple-300">Secure and private message forwarding</CardDescription>
            </CardHeader>
            <CardContent>
              <form onSubmit={handleSubmit} className="space-y-4">
                <div className="space-y-2">
                  <label htmlFor="server-select" className="text-sm font-medium">
                    Select Server
                  </label>
                  <Select onValueChange={setSelectedServer} required>
                    <SelectTrigger id="server-select" className="bg-purple-700 border-purple-600">
                      <SelectValue placeholder="Choose a server" />
                    </SelectTrigger>
                    <SelectContent className="bg-purple-700 border-purple-600">
                      {servers.map((server) => (
                        <SelectItem key={server.id} value={server.id} className="text-white">
                          {server.name}
                        </SelectItem>
                      ))}
                    </SelectContent>
                  </Select>
                </div>
                <div className="space-y-2">
                  <label htmlFor="message" className="text-sm font-medium">
                    Your Message
                  </label>
                  <Input
                    id="message"
                    value={message}
                    onChange={(e) => setMessage(e.target.value)}
                    className="bg-purple-700 border-purple-600 text-white placeholder-purple-400"
                    placeholder="Enter your message here"
                    required
                  />
                </div>
                <Button type="submit" className="w-full bg-purple-500 hover:bg-purple-400 text-purple-900">
                  <Send className="w-4 h-4 mr-2" /> Send Message
                </Button>
              </form>
            </CardContent>
            {response && (
              <CardFooter className="bg-purple-700 mt-4 rounded-b-lg">
                <div className="space-y-2 w-full">
                  <h3 className="font-semibold flex items-center gap-2">
                    <Server className="w-4 h-4" /> Server Response
                  </h3>
                  <p className="text-purple-200 text-sm">{response}</p>
                </div>
              </CardFooter>
            )}
          </Card>
        </section>
      </main>

      <footer className="bg-purple-800 py-6">
        <div className="container mx-auto px-4 text-center">
          <p>&copy; 2023 SecureProxy. All rights reserved.</p>
          <div className="mt-2">
            <a href="#" className="text-sm text-purple-300 hover:text-white mr-4">
              Privacy Policy
            </a>
            <a href="#" className="text-sm text-purple-300 hover:text-white mr-4">
              Terms of Service
            </a>
            <a href="#" className="text-sm text-purple-300 hover:text-white">
              Contact Us
            </a>
          </div>
        </div>
      </footer>
    </div>
  );
}
